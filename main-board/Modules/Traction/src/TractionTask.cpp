#include "TractionTask.h"
#include "TaskPrio.h"

TractionTask::TractionTask() : CyclicTask((char*)"Traction", TRACTION_CONTROL_CYCLE, TRACTION_TASK_PRIO, 512)
{

}

TractionTask* TractionTask::Init()
{
	static TractionTask instance;
	return &instance;
}

void TractionTask::TaskInit()
{
	controller = Traction::GetInstance();
}

void TractionTask::TaskFunction()
{
	controller->Process();
}
