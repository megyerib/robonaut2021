#pragma once

#include "ToF_4m_L1.h"
#include "Servo.h"

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
    Servo*   srv_front = nullptr;

    TOF_L1*  tof_front = nullptr;
    //TOF_L1*  tof_right;
    //TOF_L1*  tof_x;

public:
	static Distance* GetInstance();
	float GetDistance(DistanceSensor sensor); /* m */
	void SetFrontServo(float angle /* rad */);
	void Process();

private:
	bool InitDone = false;

	Distance();

	void InitPower();
	void InitServo();
	void InitTof(); // Includes waiting
	void PowerEnable(bool en);
};
