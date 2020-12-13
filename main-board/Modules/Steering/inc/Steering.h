#pragma once

// HOW TO USE
//
// 1. Get instance
// 2. Set mode
// 3. Set line (periodically)
//    Or set angle

#include "Servo.h"
#include "Pd_Controller.h"

#define STEERING_CYCLE_TIME    10 /* ms */

typedef enum
{
	Manual,

	SingleLineFollow_Fast,
	SingleLineFollow_Slow,

	SingleLine_Race_Straight,
	SingleLine_Race_Decel,
	SingleLine_Race_Turn,
	SingleLine_Race_Accel,

	SteeringReverse
	// ...
} SteeringMode;

// TODO names

typedef struct
{
    Servo*          servo;
    Pd_Controller*  controller;
    float           angle;
    float           line;
} Wheel;


class Steering
{
public:
	static Steering* GetInstance();
	void SetMode(SteeringMode mode);
	void SetLine(float front_line /* m */, float rear_line /* m */); // For LineFollow modes
	void SetAngleManual(float front_angle /* rad */); // For Free mode
	float GetFrontAngle();
	void Process();
	// TODO sensors on

private:
	SteeringMode mode;

	Wheel front;

	Steering();

	void SetFrontAngle(float angle /* rad */);

	void InitEnablePin();
};
