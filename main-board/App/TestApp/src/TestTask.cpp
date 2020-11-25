#include "TestTask.h"
#include "Steering.h"
#include "Remote.h"
#include "SteeringTask.h"
#include "TractionTask.h"

#define PRIO       6
#define PERIOD    20
#define STACK    128

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

	Steering::GetInstance()->SetMode(Free);
	Traction::GetInstance()->SetMode(tmode_Manual);
}

void TestTask::TaskFunction()
{
	float steer = Remote::GetInstance()->GetValue(SteeringCh);
	float throttle = Remote::GetInstance()->GetValue(ThrottleCh);

	Steering::GetInstance()->SetAngleManual(-0.8*steer, 0);
	Traction::GetInstance()->SetDutyCycle(0.2*throttle);
}
