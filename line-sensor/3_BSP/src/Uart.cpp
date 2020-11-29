#include "Uart.h"
#include <string.h>

Uart::Uart() : Stm32Uart(Uart1)
{
	Init();
}

Uart* Uart::GetInstance()
{
	static Uart instance;

	return &instance;
}

void Uart::Send(uint8_t* buffer, size_t size)
{
	if (size > BUFFER_MAX_SIZE)
	{
		size = BUFFER_MAX_SIZE;
	}

	memcpy(txBuffer, buffer, size);
	txBufSize = size;

	HAL_UART_Transmit_IT(&handle, txBuffer, txBufSize);
}

void Uart::TxCompleteCallback()
{

}

void Uart::RxCompleteCallback()
{

}

void Uart::Init()
{
	// GPIO --------------------------------------------------------------------

	__HAL_RCC_GPIOA_CLK_ENABLE();
	/**USART1 GPIO Configuration
	PA9      ------> USART1_TX (Swapped to Rx inside the peripheral)
	PA10     ------> USART1_RX (Swapped to Tx inside the peripheral)
	*/
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin       = GPIO_PIN_9|GPIO_PIN_10;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// USART peripheral --------------------------------------------------------

	/* USART1 clock enable */
	__HAL_RCC_USART1_CLK_ENABLE();

	handle.Instance                    = USART1;
	handle.Init.BaudRate               = 115200;
	handle.Init.WordLength             = UART_WORDLENGTH_8B;
	handle.Init.StopBits               = UART_STOPBITS_1;
	handle.Init.Parity                 = UART_PARITY_NONE;
	handle.Init.Mode                   = UART_MODE_TX_RX;
	handle.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
	handle.Init.OverSampling           = UART_OVERSAMPLING_16;
	handle.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;

#if (SENSOR_REV == 2)
	handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
	handle.AdvancedInit.Swap           = UART_ADVFEATURE_SWAP_ENABLE;
#endif

	if (HAL_UART_Init(&handle) != HAL_OK)
	{
		//Error_Handler();
	}

	// Interrupt enable --------------------------------------------------------

	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}
