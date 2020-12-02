#include <EncoderHw.h>
#include "stm32f4xx_hal.h"

EncoderHw::EncoderHw()
{
	InitGpio();
	InitTimer();

	htim.Instance->CNT = 0; // Reset timer

	HAL_TIM_Encoder_Start(&htim, TIM_CHANNEL_ALL);
}

EncoderHw& EncoderHw::GetInstance()
{
	static EncoderHw instance;
	return instance;
}

int32_t EncoderHw::GetCounterValue()
{
	return (int32_t) htim.Instance->CNT;
}

void EncoderHw::InitGpio()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOB_CLK_ENABLE();

	/**TIM2 GPIO Configuration
	PB8     ------> TIM2_CH1
	PB9     ------> TIM2_CH2
	*/
	GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP; // avoid noise when disconnected
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void EncoderHw::InitTimer()
{
	TIM_Encoder_InitTypeDef sConfig = {0};

	__HAL_RCC_TIM2_CLK_ENABLE();

	htim.Instance                   = TIM2;
	htim.Init.Prescaler             = 0;
	htim.Init.CounterMode           = TIM_COUNTERMODE_DOWN;
	htim.Init.Period                = 0xFFFFFFFF;
	htim.Init.ClockDivision         = TIM_CLOCKDIVISION_DIV1;
	htim.Init.AutoReloadPreload     = TIM_AUTORELOAD_PRELOAD_DISABLE;

	sConfig.EncoderMode             = TIM_ENCODERMODE_TI12;
	sConfig.IC1Polarity             = TIM_ICPOLARITY_RISING;
	sConfig.IC1Selection            = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC1Prescaler            = TIM_ICPSC_DIV1;
	sConfig.IC1Filter               = 0;

	sConfig.IC2Polarity             = TIM_ICPOLARITY_FALLING;
	sConfig.IC2Selection            = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC2Prescaler            = TIM_ICPSC_DIV1;
	sConfig.IC2Filter               = 0;

	HAL_TIM_Encoder_Init(&htim, &sConfig);
}
