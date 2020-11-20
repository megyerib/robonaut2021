#pragma once

#include "ToF_4m_L1.h"
#include "Servo.h"
#include "Transmitter.h"

#define DISTANCE_SAMPLING_CYCLE   50 /* ms */

typedef enum
{
	ToF_Front,
	//ToF2,
	//ToF3
}
DistanceSensor;

class Distance
{
private:
    Servo*   srv_front;

    TOF_L1*  tof_front;
    //TOF_L1*  tof_right;
    //TOF_L1*  tof_x;

    Transmitter* trace;

public:
	static Distance* GetInstance();
	float GetDistance(DistanceSensor sensor); /* m */
	void SetFrontServo(float angle /* rad */);
	void Process();

private:
	Distance();

	void InitPower();
	void PowerEnable(bool en);
};
