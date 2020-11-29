#pragma once

#include <stdint.h>
#include "stm32f4xx_hal.h"

#define UPTIME_us() Uptime::GetInstance().GetTime()

class Uptime
{
public:
	static Uptime& GetInstance();
	uint32_t GetTime(); /* us */

private:
	TIM_HandleTypeDef handle;

	Uptime();
	void InitTimer();
};
