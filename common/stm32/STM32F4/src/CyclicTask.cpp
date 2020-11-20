#include "CyclicTask.h"

typedef void (*taskFct)(void*);

CyclicTask::CyclicTask()
{

}

CyclicTask::CyclicTask(char* name, uint16_t period, uint16_t prio, uint16_t stack_size)
{
	InitTask(name, period, prio, stack_size);
}

CyclicTask::~CyclicTask()
{
	vTaskDelete(handle);
}

void CyclicTask::InitTask(char* name, uint16_t period, uint16_t prio, uint16_t stack_size)
{
	this->period = period;

	xTaskCreate(
		TaskFunctionAdapter,
		name,
		stack_size,
		this, // Parameter
		prio,
		&handle
	);
}

void CyclicTask::TaskLoop()
{
	TaskInit();

	while (1)
	{
		TaskFunction();
		vTaskDelay(period);
	}
}

// https://michaelbecker.github.io/freertos-addons/cppdocs/html/cthread_8cpp_source.html#l00201
/* static */ void CyclicTask::TaskFunctionAdapter(void* p)
{
	CyclicTask* task = (CyclicTask*)p;

	task->TaskLoop();
}
