#include "SteeringTask.h"
#include "TaskPrio.h"

SteeringTask::SteeringTask() : CyclicTask((char*)"Steering", STEERING_CYCLE_TIME, STEERING_TASK_PRIO, configMINIMAL_STACK_SIZE)
{

}

SteeringTask* SteeringTask::Init()
{
	static SteeringTask instance;
	return &instance;
}

void SteeringTask::TaskInit()
{
	steering = Steering::GetInstance();
}

void SteeringTask::TaskFunction()
{
	steering->Process();
}
