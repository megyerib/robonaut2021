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

	float GetSensorAngle(float line);

	void Follow();
	void FastLap();
	void SpeedTest();

	float SpeedPI(float v, bool reset);
};
