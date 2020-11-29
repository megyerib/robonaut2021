#include "TestTask.h"
#include "Steering.h"
#include "Remote.h"
#include "SteeringTask.h"
#include "TractionTask.h"
#include "DistanceTask.h"
#include "TraceTask.h"

#define PRIO       6
#define PERIOD    20
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

	Steering::GetInstance()->SetMode(Free);
	Traction::GetInstance()->SetMode(tmode_Manual);
}

void TestTask::TaskFunction()
{
	float steer    = Remote::GetInstance()->GetValue(SteeringCh);
	float throttle = Remote::GetInstance()->GetValue(ThrottleCh);

	Steering::GetInstance()->SetAngleManual(-0.8*steer, 0); // Front; rear
	Traction::GetInstance()->SetDutyCycle(0.2*throttle);
	Distance::GetInstance()->SetFrontServo(-0.8*steer);
}
