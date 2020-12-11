#pragma once

#include "CyclicTask.h"
#include "Servo.h"
#include "WaitTime.h"

class CalibrationTask : public CyclicTask
{
public:
	static CalibrationTask& Init();

private:
	CalibrationTask();

	void TaskInit() override;
	void TaskFunction() override;

	Servo steeringServo;
	Servo sensorServo;

	uint32_t steeringCompare = 1500;
	uint32_t sensorCompare   = 1500;

	WaitTime wait;
};
