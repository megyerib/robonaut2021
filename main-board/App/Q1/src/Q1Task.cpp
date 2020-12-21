#include "Q1Task.h"

#include "Steering.h"
#include "Remote.h"
#include "SteeringTask.h"
#include "TractionTask.h"
#include "DistanceTask.h"
#include "TraceTask.h"
#include "EncoderTask.h"
#include "TrackTask.h"
#include "TrackDetector.h"
#include "SerialMessages.h"
#include "Uptime.h"
#include "Trace.h"
#include "Distance.h"
#include "Encoder.h"
#include "WaitDistance.h"
#include <cmath>

#define PRIO        5
#define PERIOD      5
#define STACK    1024

typedef enum
{
	sStraight,
	eWaitForBraking,
	sBraking,
	sTurn
}
SPEEDRUN_STATE; // Only straight & turn

Q1Task::Q1Task() : CyclicTask((char*)"Test", PERIOD, PRIO, STACK)
{

}

Q1Task& Q1Task::Init()
{
	static Q1Task instance;
	return instance;
}

void Q1Task::TaskInit()
{
	SteeringTask::Init();
	TractionTask::Init();
	TraceTask::Init();
	DistanceTask::Init();
	EncoderTask::Init();
	TrackTask::Init();

	// TODO szóljon be, ha nem raktuk be semmilyen módba.

	TrackDetector::GetInstance()->SetMode(Speedrun);
	Steering::GetInstance()->SetMode(SingleLine_Race_Straight);
	Traction::GetInstance()->SetMode(tmode_Manual);
}

void Q1Task::TaskFunction()
{
	//Follow();
	FastLap();

	//ID_LowSpeed();
	//ID_HighSpeed();
}

float Q1Task::GetSensorAngle(float line)
{
	// see distance-sensor-angle.xlsx
	float ret = 0;

	if (abs(line) > 0.02)
	{
		ret = 0.34 + 1.3 * abs(line);
		ret *= (line > 0) ? -1 : 1;
	}

	return ret;
}

void Q1Task::Follow() // Now speed controller test TODO rewrite
{
	float frontLine = TrackDetector::GetInstance()->GetFrontLine();
	float throttle = Remote::GetInstance().GetValue(chThrottle);

	Steering* steering = Steering::GetInstance();
	steering->SetLine(frontLine, 0);

	// Distance
	/*Distance* dstSensor = Distance::GetInstance();
	dstSensor->SetFrontServo(steering->GetFrontAngle());
	float dist = dstSensor->GetDistance(ToF_Front);
	TRACE_DUMMY(dist * 1000);*/

	float speed = Encoder::GetInstance().GetSpeed();
	TRACE_DUMMY(speed * 1000);

	float d;

	if (throttle > 0.15f)
	{
		d = SpeedController(1.5f);
	}
	else
	{
		d = 0.0f;
		//d = SpeedController(0.0f);
	}

	Traction::GetInstance()->SetDutyCycle(d);
}

void Q1Task::FastLap()
{
	// 1 Read & trace speed (required for braking)
	float speed = Encoder::GetInstance().GetSpeed();
	TRACE_DUMMY(speed * 1000);

	// 2 Read road signal
	TrackType roadSignal = TrackDetector::GetInstance()->GetTrackType();

	// 3 Speedrun state machine (set d & line following controller)
	static SPEEDRUN_STATE FastLapState = sStraight;

	static WaitDistance waitDst;
	static float targetSpeed = 1.65f;
	static const float turnSpeed = 1.65f; // m/s
	static const float straightSpeed = 4.0f; // m/s

	switch (FastLapState)
	{
		case sStraight:
		{
			if (roadSignal == Braking)
			{
				waitDst.Wait_m(2.0f);
				FastLapState = eWaitForBraking;
			}

			break;
		}
		case eWaitForBraking:
		{
			if (waitDst.IsExpired())
			{
				PRINTF("Braking");
				targetSpeed = turnSpeed; // Set turn speed by controller
				FastLapState = sBraking;
			}
			break;
		}
		case sBraking:
		{
			PRINTF("Turning");
			FastLapState = sTurn;

			Steering::GetInstance()->SetMode(SingleLine_Race_Turn);

			break;
		}
		case sTurn:
		{
			if (roadSignal == Acceleration)
			{
				PRINTF("Accel");
				targetSpeed = straightSpeed;
				Steering::GetInstance()->SetMode(SingleLine_Race_Straight);
				FastLapState = sStraight;
			}
		}
	}

	// 4 Set line (& get average of front lines)
	{
		static float frontLines[3];
		static size_t iFrontLine = 0;

		frontLines[iFrontLine] = TrackDetector::GetInstance()->GetFrontLine();
		iFrontLine++;
		iFrontLine %= 3;

		float frontLine = (frontLines[0] + frontLines[1] + frontLines[2]) / 3;
		Steering::GetInstance()->SetLine(frontLine, 0);
	}

	// 5 Set motor duty cycle if enabled
	float remThr = Remote::GetInstance().GetValue(chThrottle);
	float d;

	if (remThr > 0.15f)
	{
		d = SpeedController(targetSpeed);
	}
	else
	{
		d = 0; // Dont calculate integrated error TODO solve this later properly
	}

	Traction::GetInstance()->SetDutyCycle(d);
}

#define SATURATE(x, min, max)  ((x) = (x) > (max) ? (max) : ((x) < (min) ? (min) : (x)))


#define T_s 0.01f

#define K   14.78f
#define T   0.55f
#define p_d exp(-T_s/T)
#define K_d (K * (1 - exp(-T_s/T)))
#define T_Cl 0.25f
#define K_C (1.0 / K_d * (1.0 - exp(-T_s/T_Cl)))
#define z_d p_d
#define MAX_u 0.5f

float Q1Task::SpeedController(float r)
{
	float y = Encoder::GetInstance().GetSpeed();

	static float u_prev;
	static float u_2prev;

	float e = r - y;
	float u_1 = K_C * e;
	float u_2 = z_d * u_2prev + (1.0 - z_d) * u_prev;
	float u = u_1 + u_2;
	SATURATE(u, -MAX_u, MAX_u);

	u_prev  = u;
	u_2prev = u_2;

	return u;
}

void Q1Task::ID_LowSpeed()
{
	float frontLine = TrackDetector::GetInstance()->GetFrontLine();

	float speed = Encoder::GetInstance().GetSpeed();
	TRACE_DUMMY(speed * 1000);

	float remThr = Remote::GetInstance().GetValue(chThrottle);

	Steering::GetInstance()->SetLine(frontLine, 0);

	float d = 0;

	static float d_max = 0.0f;
	static bool running = false;

	if (remThr > 0.15f)
	{
		if (!running)
		{
			running = true;
			d_max += 0.05f;
			PRINTF("Speed: %d", (d_max * 100.0));
		}

		d = d_max;
	}
	else if (remThr < 0.05f)
	{
		d = 0.0f;

		if (running)
		{
			running = false;
		}
	}
	else
	{
		d = 0.0f;
	}

	Traction::GetInstance()->SetDutyCycle(d);
}

void Q1Task::ID_HighSpeed()
{
	// 1 Read & trace speed (required for braking)
	float speed = Encoder::GetInstance().GetSpeed();
	TRACE_DUMMY(speed * 1000);

	// 2 Read road signal
	TrackType roadSignal = TrackDetector::GetInstance()->GetTrackType();

	// 3 Speedrun state machine (set d & line following controller)
	static SPEEDRUN_STATE FastLapState = sStraight;

	static WaitDistance waitDst;
	static float d = 0.2f;
	static const float turnSpeed = 0.2f;
	static float straightSpeed = 0.2f;

	switch (FastLapState)
	{
		case sStraight:
		{
			if (roadSignal == Braking)
			{
				waitDst.Wait_m(0.0f); // Don't wait during this test
				FastLapState = eWaitForBraking;
			}

			break;
		}
		case eWaitForBraking:
		{
			if (waitDst.IsExpired())
			{
				PRINTF("Braking");
				d = 0.0f;
				FastLapState = sBraking;
			}
			break;
		}
		case sBraking:
		{
			if (speed < 2.0f)
			{
				PRINTF("Turning");
				d = turnSpeed;
				FastLapState = sTurn;
			}

			Steering::GetInstance()->SetMode(SingleLine_Race_Turn);

			break;
		}
		case sTurn:
		{
			if (roadSignal == Acceleration)
			{
				PRINTF("Accel");
				straightSpeed += 0.05f;
				d = straightSpeed;
				PRINTF("d = %d", (int32_t)(d * 100.0f));
				Steering::GetInstance()->SetMode(SingleLine_Race_Straight);
				FastLapState = sStraight;
			}
		}
	}

	// 4 Set line (& get average of front lines)
	{
		static float frontLines[3];
		static size_t iFrontLine = 0;

		frontLines[iFrontLine] = TrackDetector::GetInstance()->GetFrontLine();
		iFrontLine++;
		iFrontLine %= 3;

		float frontLine = (frontLines[0] + frontLines[1] + frontLines[2]) / 3;
		Steering::GetInstance()->SetLine(frontLine, 0);
	}

	// 5 Set motor duty cycle if enabled
	float remThr = Remote::GetInstance().GetValue(chThrottle);
	Traction::GetInstance()->SetDutyCycle((remThr > 0.15f) ? d : 0);
}

// Imaginary: What we want to achieve
// Real: What the controller can push out
float Q1Task::ReSpeedToIm(float realSpeed)
{
	if (realSpeed >= 0.07)
	{
		return realSpeed * 14.78 - 1.03;
	}
	else if (realSpeed < -0.07)
	{
		realSpeed *= -1.0;
		return -(realSpeed * 14.78 - 1.03);
	}
	else
	{
		return 0.0f;
	}
}

float Q1Task::ImSpeedToRe(float imSpeed) // TODO inverse bad
{
	if (imSpeed >= 0)
	{
		return imSpeed * 0.068 + 0.07;
	}
	else
	{
		imSpeed *= -1.0;
		return -(imSpeed * 0.068 + 0.07);
	}
}
