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
	float remSteering = Remote::GetInstance().GetValue(chSteering);
	bool remoteMode = Remote::GetInstance().GetMode();

	//TRACE_REMOTE(throttle * 100, remSteering * 100, remoteMode * 100);

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
	static bool straight = true;

	TrackType roadSignal = TrackDetector::GetInstance()->GetTrackType();

	if (straight == true && roadSignal == Braking)
	{
		PRINTF("Braking");
		straight = false;
	}
	if (straight == false && roadSignal == Acceleration)
	{
		PRINTF("Accel");
		straight = true;
	}

	float frontLine = TrackDetector::GetInstance()->GetFrontLine();
	TRACE_DUMMY(frontLine * 1000000);

	float throttle = Remote::GetInstance().GetValue(chThrottle);

	Steering::GetInstance()->SetLine(frontLine, 0);

	float d = 0;

	if (throttle > 0.15f)
	{
		d = 0.15f;
	}
	else
	{
		d = 0.0f;
	}

	Traction::GetInstance()->SetDutyCycle(d);
}
