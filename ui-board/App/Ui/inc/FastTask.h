#pragma once

#include "CyclicTask.h"
#include <cstdint>

class FastTask : CyclicTask
{
public:
	static FastTask& Init();

private:
	FastTask();

	void TaskInit() override;
	void TaskFunction() override;
};
