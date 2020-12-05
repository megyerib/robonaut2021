#pragma once

#include "CyclicTask.h"
#include <cstdint>

class Line2020Task : CyclicTask
{
public:
	static Line2020Task& Init();

private:
	Line2020Task();

	void TaskInit() override;
	void TaskFunction() override;
};
