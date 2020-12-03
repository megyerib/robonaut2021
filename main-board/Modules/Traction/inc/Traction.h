#pragma once

#include "Pid_Controller.h"
#include <cstddef> /* size_t */

#define TRACTION_CONTROL_CYCLE   10 /* ms */

typedef enum
{
    tmode_Manual = 0,
    tmode_Controller,
	tmodeAccel,
	tmodeBraking
	// ...
}
TractionMode;

class Traction
{
public:
	static Traction* GetInstance();
	void SetSpeed(float speed /* m/s */);
	void SetDutyCycle(float d /* % [-1;+1] */);
	void SetMode(TractionMode mode);
	void SetControllerIntegrationLimit(float const limit);

	void Process();

private:
	Pid_Controller* controller;

	//float targetSpeed;
	float targetDutyCycle;
	float prevDutyCycle;
	float forceDutyCycle;

	TractionMode mode;

	Traction();

	void SendDutyCycle(float d /* % [-1;+1] */);
	void PrintInt(void* dst, int n, size_t& size);
};
