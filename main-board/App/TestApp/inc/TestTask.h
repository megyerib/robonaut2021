#pragma once

#include "CyclicTask.h"

class TestTask : public CyclicTask
{
public:
	static TestTask& Init();

private:
	TestTask();

	void TaskInit() override;
	void TaskFunction() override;

	float GetSensorAngle(float line);
};
