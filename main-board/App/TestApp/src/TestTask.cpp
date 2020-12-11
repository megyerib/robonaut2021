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

	if (throttle > 0.15f)
	{
		Traction::GetInstance()->SetDutyCycle(0.15);
	}
	else
	{
		Traction::GetInstance()->SetDutyCycle(0);
	}

	// Trace line
	SM_DUMMY msg;
	msg.timestamp = UPTIME_us();
	msg.value = frontLine * 100;

	TRACE_BIN(&msg, sizeof(msg));

	// Steering::GetInstance()->SetAngleManual(-0.8*steer, 0); // Front; rear
	// Distance::GetInstance()->SetFrontServo(-0.8*steer);
}
