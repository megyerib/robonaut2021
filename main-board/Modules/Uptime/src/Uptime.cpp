#include "Uptime.h"

Uptime::Uptime()
{
	Init();
	HAL_TIM_Base_Start(&handle);
}

Uptime* Uptime::GetInstance()
{
	static Uptime instance;
	return &instance;
}

uint32_t Uptime::GetTime()
{
	return TIM5->CNT;
}

void Uptime::Init()
{
	__HAL_RCC_TIM5_CLK_ENABLE();

	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	handle.Instance = TIM5;
	handle.Init.Prescaler = 90-1; // f_APB1 = 90 MHz
	handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	handle.Init.Period = 0xFFFFFFFF;
	handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_OC_Init(&handle) != HAL_OK)
	{
		//Error_Handler();
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	if (HAL_TIMEx_MasterConfigSynchronization(&handle, &sMasterConfig) != HAL_OK)
	{
		//Error_Handler();
	}

	sConfigOC.OCMode = TIM_OCMODE_TIMING;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	if (HAL_TIM_OC_ConfigChannel(&handle, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		//Error_Handler();
	}
}
