#include <MainUart.h>
#include "SensorCfg.h"

#define DMA_NVIC_PRIO         2
#define TRACE_UART_NVIC_PRIO  2

// Common data
#define CLASS_NAME           MainUart
#define RX_BUF_SIZE             (50u)
#define BAUDRATE           (1000000u)

static uint8_t rxBuffer[RX_BUF_SIZE];

const DMA_UART_CFG uart_cfg =
{
	// Tx
	.txEnabled     = true,

	// Rx
	.rxEnabled     = true,
	.rxBuf         = rxBuffer,
	.rxBufSize     = RX_BUF_SIZE,

	// GPIO
	.gpioTxClkEn   = [](){__HAL_RCC_GPIOA_CLK_ENABLE();},
	.gpioTxPort    = GPIOA,
	.gpioTxPin     = GPIO_PIN_9,

	.gpioRxClkEn   = [](){__HAL_RCC_GPIOA_CLK_ENABLE();},
	.gpioRxPort    = GPIOA,
	.gpioRxPin     = GPIO_PIN_10,

	.gpioAf        = GPIO_AF1_USART1,

	// DMA
	.dmaClkEn      = [](){__HAL_RCC_DMA1_CLK_ENABLE();},

	.dmaTxChannel  = DMA1_Channel2,
	.dmaTxIrq      = DMA1_Channel2_3_IRQn,
	.dmaTxNvicPrio = DMA_NVIC_PRIO,

	.dmaRxChannel  = DMA1_Channel3,
	.dmaRxIrq      = DMA1_Channel2_3_IRQn,
	.dmaRxNvicPrio = DMA_NVIC_PRIO,

	// UART
	.uartClkEn     = [](){__HAL_RCC_USART1_CLK_ENABLE();},
	.uartInstance  = USART1,
	.uartBaudRate  = BAUDRATE,
	.uartIrq       = USART1_IRQn,
	.uartNvicPrio  = TRACE_UART_NVIC_PRIO,

#if (SENSOR_REV == 2)
	.uartSwapPins  = true
#else
	.uartSwapPins  = false
#endif
};

// Don't touch ---------------------------------------------------------------------------------------------------------

CLASS_NAME& CLASS_NAME::GetInstance()
{
	static CLASS_NAME instance;
	return instance;
}

CLASS_NAME::CLASS_NAME() : DmaUartF0(uart_cfg)
{

}

// Interrupt handlers --------------------------------------------------------------------------------------------------

extern "C" void DMA1_CH2_3_IRQHandler(void)
{
	CLASS_NAME::GetInstance().HandleDmaRxIrq();
	CLASS_NAME::GetInstance().HandleDmaTxIrq();
}

extern "C" void USART1_IRQHandler(void)
{
	CLASS_NAME::GetInstance().HandleUartIrq();
}

// End -----------------------------------------------------------------------------------------------------------------
