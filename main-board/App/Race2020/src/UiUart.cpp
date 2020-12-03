#include "UiUart.h"

#include <cstring>
#include "NvicPrio.h"

UiUart::UiUart()
{
	InitGpio();
	InitUart();
}

UiUart* UiUart::GetInstance()
{
	static UiUart instance;
	return &instance;
}

int32_t UiUart::Receive(void* buffer, size_t& size, size_t targetSize)
{
	if (buffer == nullptr)
	{
		return RECEIVE_PARAM_ERROR;
	}
	if (targetSize < 1)
	{
		return RECEIVE_INSUFFICIENT_TARGET_SIZE;
	}

	if (huart.Instance->SR & USART_SR_RXNE)
	{
		*((uint8_t*)buffer) = huart.Instance->DR;
		size = 1;
	}
	else
	{
		size = 0;
	}

	return RECEIVE_OK;
}

int32_t UiUart::Transmit(const void* buffer, size_t size)
{
	HAL_UART_Transmit(&huart, (uint8_t*)buffer, 1, 0);

	return TRANSMIT_OK;
}

void UiUart::InitUart()
{
	__HAL_RCC_USART1_CLK_ENABLE();

	huart.Instance              = USART1;
	huart.Init.BaudRate         = 115200;
	huart.Init.WordLength       = UART_WORDLENGTH_8B;
	huart.Init.StopBits         = UART_STOPBITS_1;
	huart.Init.Parity           = UART_PARITY_NONE;
	huart.Init.Mode             = UART_MODE_TX_RX;
	huart.Init.HwFlowCtl        = UART_HWCONTROL_NONE;
	huart.Init.OverSampling     = UART_OVERSAMPLING_16;

	HAL_UART_Init(&huart);
}

void UiUart::InitGpio()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin          = GPIO_PIN_9|GPIO_PIN_10;
	GPIO_InitStruct.Mode         = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull         = GPIO_PULLUP;
	GPIO_InitStruct.Speed        = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate    = GPIO_AF7_USART1;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
