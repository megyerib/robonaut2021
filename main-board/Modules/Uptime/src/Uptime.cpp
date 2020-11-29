#include "Uptime.h"

Uptime::Uptime()
{
	InitTimer();

	// Start timer
	HAL_TIM_Base_Start(&handle);
}

Uptime& Uptime::GetInstance()
{
	static Uptime instance;
	return instance;
}

uint32_t Uptime::GetTime()
{
	return __HAL_TIM_GET_COUNTER(&handle);
}

void Uptime::InitTimer()
{
	__HAL_RCC_TIM5_CLK_ENABLE();

	handle.Instance               = TIM5;
	handle.Init.Prescaler         = 90-1; // f_APB1 = 90 MHz
	handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	handle.Init.Period            = 0xFFFFFFFF;
	handle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	HAL_TIM_OC_Init(&handle);
}
