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

#define PRIO       6
#define PERIOD    5
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
	Steering::GetInstance()->SetMode(SingleLineFollow_Slow);
	Traction::GetInstance()->SetMode(tmode_Manual);
}

#define CONST_MOTOR_D  20

void TestTask::TaskFunction()
{
	float frontLine = TrackDetector::GetInstance()->GetFrontLine();

	//float steer    = Remote::GetInstance().GetValue(chSteering);
	float throttle = Remote::GetInstance().GetValue(chThrottle);

	Steering::GetInstance()->SetLine(frontLine, 0);

	// Steering::GetInstance()->SetAngleManual(-0.8*steer, 0); // Front; rear

	Distance* dstSensor = Distance::GetInstance();

	float sensorAngle = GetSensorAngle(frontLine);

	dstSensor->SetFrontServo(sensorAngle);
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
			d = (dist - 0.5f) * 0.15f;
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

	// Trace line
	SM_DUMMY msg;
	msg.timestamp = UPTIME_us();
	msg.value = d * 100.0;

	TRACE_BIN(&msg, sizeof(msg));

	Traction::GetInstance()->SetDutyCycle(d);
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
