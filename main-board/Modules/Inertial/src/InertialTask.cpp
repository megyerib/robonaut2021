#include "InertialTask.h"
#include "Inertial.h"

#define PERIOD     10
#define PRIO       3
#define STACK      configMINIMAL_STACK_SIZE

InertialTask::InertialTask() : CyclicTask((char*)"Inert", PERIOD, PRIO, STACK)
{

}

InertialTask& InertialTask::Init()
{
    static InertialTask instance;
    return instance;
}

void InertialTask::TaskInit()
{

}

void InertialTask::TaskFunction()
{
	Inertial::GetInstance().Process();
}
