#include "StarterUart.h"
#include <cstring>
#include "NvicPrio.h"

StarterUart::StarterUart()
{
	InitGpio();
	InitUart();
}

StarterUart* StarterUart::GetInstance()
{
	static StarterUart instance;
	return (&instance);
}

int32_t StarterUart::Receive(void* buffer, size_t& size, size_t targetSize)
{
	if (buffer == nullptr)
	{
		return RECEIVE_BUFFER_NULL;
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

void StarterUart::InitUart()
{
	__HAL_RCC_UART4_CLK_ENABLE();

	huart.Instance              = UART4;
	huart.Init.BaudRate         = 115200;
	huart.Init.WordLength       = UART_WORDLENGTH_8B;
	huart.Init.StopBits         = UART_STOPBITS_1;
	huart.Init.Parity           = UART_PARITY_NONE;
	huart.Init.Mode             = UART_MODE_RX;
	huart.Init.HwFlowCtl        = UART_HWCONTROL_NONE;
	huart.Init.OverSampling     = UART_OVERSAMPLING_16;

	HAL_UART_Init(&huart);
}

void StarterUart::InitGpio()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/**USART4 GPIO Configuration
	PA0      ------> UART4_TX
	PA1      ------> UART4_RX
	*/
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin          = GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Mode         = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull         = GPIO_PULLUP;
	GPIO_InitStruct.Speed        = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate    = GPIO_AF8_UART4;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
