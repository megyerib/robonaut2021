#pragma once

#include "CyclicTask.h"

class Q2Task : public CyclicTask
{
public:
	static Q2Task& Init();

private:
	Q2Task();

	void TaskInit() override;
	void TaskFunction() override;
};
