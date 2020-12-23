#pragma once

#include <stdint.h>
#include "stm32f0xx_hal.h"

#define UPTIME_us() Uptime::GetInstance().GetTime()

class Uptime
{
public:
	static Uptime& GetInstance();
	uint32_t GetTime(); /* us */

private:
	TIM_HandleTypeDef htim_low;  // Master
	TIM_HandleTypeDef htim_high; // Slave

	Uptime();
	void InitMasterTimer();
	void InitSlaveTimer();
};
