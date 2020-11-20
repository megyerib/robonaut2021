#pragma once

#include "stm32f4xx_hal.h"

class EncoderHw
{
public:
	static EncoderHw* GetInstance();
	uint32_t GetCounterValue();

private:
	TIM_HandleTypeDef htim;

	EncoderHw();

	void InitGpio();
	void InitTimer();
};
