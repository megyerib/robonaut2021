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

	// TODO to identification task
	void ID_LowSpeed();
	void ID_HighSpeed();

	float ReSpeedToIm(float realSpeed);
	float ImSpeedToRe(float imSpeed);

	float SpeedController(float v);
};
