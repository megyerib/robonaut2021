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

#define PRIO        5
#define PERIOD      5
#define STACK    1024

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
	FastLap();
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

void Q1Task::Follow()
{
	float frontLine = TrackDetector::GetInstance()->GetFrontLine();
	float throttle = Remote::GetInstance().GetValue(chThrottle);

	Steering* steering = Steering::GetInstance();
	steering->SetLine(frontLine, 0);

	Distance* dstSensor = Distance::GetInstance();
	dstSensor->SetFrontServo(steering->GetFrontAngle());
	float dist = dstSensor->GetDistance(ToF_Front);
	TRACE_DUMMY(dist * 1000);

	float speed = Encoder::GetInstance().GetSpeed();
	//TRACE_DUMMY(speed * 1000);

	float d = 0;

	static bool started = false;

	if (throttle > 0.15f)
	{
		if (!started)
		{
			// Soft start
			d = 0.2f;
			if (speed > 1.0f)
				started = true;
		}
		else
		{
			if (dist < 0.25f)
			{
				d = SpeedPI(0.0f, false);
			}
			else if (dist < 1.0f)
			{
				d = SpeedPI((dist - 0.25f) * 1.5f, false);
			}
			else
			{
				d = SpeedPI(1.5f, false);
			}
		}
	}
	else
	{
		d = SpeedPI(0.0f, false);
		started = false;
	}

	Traction::GetInstance()->SetDutyCycle(d);
}

void Q1Task::FastLap()
{
	static enum
	{
		sStraight,
		eWaitForBraking,
		sBraking,
		sTurn
	}
	FastLapState = sStraight;

	static float maxThrottle = 0.2f;
	static WaitDistance waitDst;

	TrackType roadSignal = TrackDetector::GetInstance()->GetTrackType();
	float speed = Encoder::GetInstance().GetSpeed();

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
				maxThrottle = 0.0f;
				FastLapState = sBraking;
			}
			break;
		}
		case sBraking:
		{
			if (speed < 2.0f)
			{
				PRINTF("Turning");
				maxThrottle = 0.2f;
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
				maxThrottle = 0.35f;
				Steering::GetInstance()->SetMode(SingleLine_Race_Straight);
				FastLapState = sStraight;
			}
		}
	}

	static float frontLines[3];
	static size_t iFrontLine = 0;

	frontLines[iFrontLine] = TrackDetector::GetInstance()->GetFrontLine();
	iFrontLine++;
	iFrontLine %= 3;

	float frontLine = (frontLines[0] + frontLines[1] + frontLines[2]) / 3;

	TRACE_DUMMY(speed * 1000);

	//TRACE_DUMMY(frontLine * 1000000);

	float remThr = Remote::GetInstance().GetValue(chThrottle);

	Steering::GetInstance()->SetLine(frontLine, 0);

	float d = 0;

	if (remThr > 0.15f)
	{
		d = maxThrottle;
	}
	else
	{
		d = 0.0f;
	}

	Traction::GetInstance()->SetDutyCycle(d);
}

void Q1Task::SpeedTest()
{
	float frontLine = TrackDetector::GetInstance()->GetFrontLine();
	float throttle = Remote::GetInstance().GetValue(chThrottle);

	Steering* steering = Steering::GetInstance();
	steering->SetLine(frontLine, 0);

	float speed = Encoder::GetInstance().GetSpeed();
	TRACE_DUMMY(speed * 1000);

	float d = 0;

	static bool started = false;

	if (throttle > 0.15f)
	{
		if (!started)
		{
			d = 0.2f;
			if (speed > 1.0f)
				started = true;
		}
		else
		{
			d = SpeedPI(1.5f, false);
		}
	}
	else
	{
		d = SpeedPI(0.0f, false);
		started = false;
	}

	Traction::GetInstance()->SetDutyCycle(d);
}

#define d_MAX   0.5f
#define d_MIN  -0.5f
#define K_P  0.25f
#define K_I  0.01f

#define SATURATE(x, min, max)  ((x) = (x) > (max) ? (max) : ((x) < (min) ? (min) : (x)))

float Q1Task::SpeedPI(float v, bool reset)
{
	float d;

	float speed = Encoder::GetInstance().GetSpeed();
	float error = v - speed;

	static float cumulativeError = 0.0f;
	if (reset)
	{
		cumulativeError = 0;
	}
	cumulativeError += error;

	d = /*0.0789 * v + 0.058 +*/ K_P * error + K_I * cumulativeError;

	SATURATE(d, d_MIN, d_MAX);

	// TODO max delta

	return d;
}
