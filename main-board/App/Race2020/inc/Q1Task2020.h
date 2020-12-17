#pragma once

#include "CyclicTask.h"

#include "Remote.h"
#include "Traction.h"
#include "Steering.h"
#include "Car.h"

class Q1Task2020 : public CyclicTask
{
public:
	static Q1Task2020* Init();

private:
	Q1Task2020();

	Car*      car       = nullptr;
	Remote&   remote;
	Traction* motor     = nullptr;
	Steering* steering  = nullptr;

	void TaskInit() override;
	void TaskFunction() override;

	void RcRun(float throttle, float steerAngle);
};
