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
	Off,
	DualLineFollow_Slow,
	DualLineFollow_Fast,
	SingleLineFollow_Fast,
	SingleLineFollow_Slow,

	DualLine_Race_Straight,
	DualLine_Race_Turn,

	SingleLine_Race_Straight,
	SingleLine_Race_Decel,
	SingleLine_Race_Turn,
	SingleLine_Race_Accel,

	Free,             // Angles are set manually
	SteeringReverse
	// ...
} SteeringMode;

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
	void SetAngleManual(float front_angle /* rad */, float rear_angle /* rad */); // For Free mode
	float GetFrontAngle();
	void Process();

private:
	SteeringMode mode;

	Wheel front;
	Wheel rear;

	Steering();

	void SetFrontAngle(float angle /* rad */);
	void SetRearAngle(float angle /* rad */);

	void InitEnablePin();
};
