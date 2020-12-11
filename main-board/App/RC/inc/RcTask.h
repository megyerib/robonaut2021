#pragma once

#include "CyclicTask.h"

class RcTask : public CyclicTask
{
public:
	static RcTask& Init();

private:
	RcTask();

	void TaskInit() override;
	void TaskFunction() override;
};
