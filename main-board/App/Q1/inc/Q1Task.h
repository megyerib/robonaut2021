#pragma once

#include "CyclicTask.h"

class Q1Task : public CyclicTask
{
public:
	static Q1Task& Init();

private:
	Q1Task();

	void TaskInit() override;
	void TaskFunction() override;

	void FastLap();
	void Follow();

	float SpeedPI(float v, bool reset); // TODO move to separate file & replace to FOXBORO
};
