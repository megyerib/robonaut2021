#include "FastTask.h"
#include "Matrix.h"
#include "Buttons.h"

#define PRIO                 3
#define PERIOD               1
#define STACK              configMINIMAL_STACK_SIZE

FastTask::FastTask() : CyclicTask((char*)"Fast", PERIOD, PRIO, STACK)
{

}

FastTask& FastTask::Init()
{
	static FastTask instance;
	return instance;
}

void FastTask::TaskInit()
{

}

void FastTask::TaskFunction()
{
	Matrix::GetInstance().Refresh();
	Buttons::GetInstance().Process();
}
