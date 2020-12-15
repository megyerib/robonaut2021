#include "TestTask.h"
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

TestTask::TestTask() : CyclicTask((char*)"Test", PERIOD, PRIO, STACK)
{

}

TestTask& TestTask::Init()
{
	static TestTask instance;
	return instance;
}

void TestTask::TaskInit()
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

void TestTask::TaskFunction()
{
	FastLap();
}

float TestTask::GetSensorAngle(float line)
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

void TestTask::Follow()
{
	float frontLine = TrackDetector::GetInstance()->GetFrontLine();

	float throttle = Remote::GetInstance().GetValue(chThrottle);

	Steering* steering = Steering::GetInstance();
	steering->SetLine(frontLine, 0);

	Distance* dstSensor = Distance::GetInstance();

	//float sensorAngle = GetSensorAngle(frontLine);

	dstSensor->SetFrontServo(steering->GetFrontAngle()*0.5f);
	float dist = dstSensor->GetDistance(ToF_Front);

	float d = 0;

	if (throttle > 0.15f)
	{
		if (dist < 0.5f)
		{
			d = 0;
		}
		else if (dist < 1.5f)
		{
			d = (dist - 0.5) * 0.15f;
		}
		else
		{
			d = 0.15f;
		}
	}
	else
	{
		d = 0.0f;
	}

	Traction::GetInstance()->SetDutyCycle(d);
}

void TestTask::FastLap()
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
