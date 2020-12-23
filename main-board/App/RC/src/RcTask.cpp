#include "RcTask.h"
#include "Remote.h"
#include "SteeringTask.h"
#include "TractionTask.h"


#define PRIO     6
#define PERIOD   10
#define STACK    configMINIMAL_STACK_SIZE

#define THROTTLE_FUN_FACTOR    0.4f

RcTask::RcTask() : CyclicTask((char*)"RC", PERIOD, PRIO, STACK)
{

}

RcTask& RcTask::Init()
{
	static RcTask instance;
	return instance;
}

void RcTask::TaskInit()
{
	SteeringTask::Init();
	TractionTask::Init();

	Steering::GetInstance()->SetMode(smManual);
	Traction::GetInstance()->SetMode(tmDutyCycle);
}

void RcTask::TaskFunction()
{
	float steer    = Remote::GetInstance().GetValue(chSteering);
	float throttle = Remote::GetInstance().GetValue(chThrottle);

	float steerAngle = (steer * -60.0) * (3.1416f / 180.0f); // Max 60°; rad

	Steering::GetInstance()->SetAngleManual(steerAngle);
	Traction::GetInstance()->SetDutyCycle(throttle * THROTTLE_FUN_FACTOR);
}
