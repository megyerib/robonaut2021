#pragma once

#include "CyclicTask.h"

class SpeedTestTask : public CyclicTask
{
public:
	static SpeedTestTask& Init();

private:
	SpeedTestTask();

	void TaskInit() override;
	void TaskFunction() override;

	void SpeedControllerTest();
	void ID_LowSpeed();
	void ID_HighSpeed();
	float ReSpeedToIm(float realSpeed);
	float ImSpeedToRe(float imSpeed);
	float SpeedController(float v);
};
