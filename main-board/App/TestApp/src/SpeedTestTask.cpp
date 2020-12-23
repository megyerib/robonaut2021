#include "SpeedTestTask.h"

#include "Steering.h"
#include "Remote.h"
#include "SteeringTask.h"
#include "TractionTask.h"
#include "TraceTask.h"
#include "EncoderTask.h"
#include "TrackTask.h"
#include "TrackDetector.h"
#include "SerialMessages.h"
#include "Uptime.h"
#include "Trace.h"
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

SpeedTestTask::SpeedTestTask() : CyclicTask((char*)"Test", PERIOD, PRIO, STACK)
{

}

SpeedTestTask& SpeedTestTask::Init()
{
	static SpeedTestTask instance;
	return instance;
}

void SpeedTestTask::TaskInit()
{
	SteeringTask::Init();
	TractionTask::Init();
	TraceTask::Init();
	EncoderTask::Init();
	TrackTask::Init();

	TrackDetector::GetInstance()->SetMode(trackSpeedrun);
	Steering::GetInstance()->SetMode(smStraight);
	Traction::GetInstance()->SetMode(tmDutyCycle);
}

void SpeedTestTask::TaskFunction()
{
	SpeedControllerTest();
}

void SpeedTestTask::SpeedControllerTest()
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

float SpeedTestTask::SpeedController(float r)
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

void SpeedTestTask::ID_LowSpeed()
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

void SpeedTestTask::ID_HighSpeed()
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

			Steering::GetInstance()->SetMode(smTurn);

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
				Steering::GetInstance()->SetMode(smStraight);
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
float SpeedTestTask::ReSpeedToIm(float realSpeed)
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

float SpeedTestTask::ImSpeedToRe(float imSpeed)
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
