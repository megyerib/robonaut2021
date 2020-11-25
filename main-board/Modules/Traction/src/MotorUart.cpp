#include "MotorUart.h"
#include "NvicPrio.h"

#define RX_BUF_SIZE (1u) /* No Rx */
#define UART_IRQ_HANDLER    USART2_IRQHandler
#define DMA_TX_IRQ_HANDLER  DMA1_Stream6_IRQHandler
//#define DMA_RX_IRQ_HANDLER  DMA1_Stream5_IRQHandler
#define CLASS_NAME          MotorUart

static uint8_t rxBuffer[RX_BUF_SIZE];

static DMA_UART_CFG uart_cfg =
{
	.txEnabled     = true,
	.rxEnabled     = false, // There is no free DMA stream for Rx

	// Rx buffer
	.rxBuf         = rxBuffer,
	.rxBufSize     = RX_BUF_SIZE,

	// GPIO
	.gpioTxClkEn   = [](){__HAL_RCC_GPIOA_CLK_ENABLE();},
	.gpioTxPort    = GPIOA,
	.gpioTxPin     = GPIO_PIN_2,

	.gpioRxClkEn   = [](){__HAL_RCC_GPIOA_CLK_ENABLE();},
	.gpioRxPort    = GPIOA,
	.gpioRxPin     = GPIO_PIN_3,

	.gpioAf        = GPIO_AF7_USART2,

	// DMA
	.dmaClkEn      = [](){__HAL_RCC_DMA1_CLK_ENABLE();},

	.dmaTxStream   = DMA1_Stream6,
	.dmaTxChannel  = DMA_CHANNEL_4,
	.dmaTxIrq      = DMA1_Stream6_IRQn,
	.dmaTxNvicPrio = DMA_NVIC_PRIO,

	.dmaRxStream   = DMA1_Stream5,
	.dmaRxChannel  = DMA_CHANNEL_4,
	.dmaRxIrq      = DMA1_Stream5_IRQn,
	.dmaRxNvicPrio = DMA_NVIC_PRIO,

	// UART
	.uartClkEn     = [](){__HAL_RCC_USART2_CLK_ENABLE();},
	.uartInstance  = USART2,
	.uartBaudRate  = 115200,
	.uartIrq       = USART2_IRQn,
	.uartNvicPrio  = DMA_NVIC_PRIO, // TODO all of these
};

// No touching needed ----------------------------------------------------------

CLASS_NAME* CLASS_NAME::GetInstance()
{
	static CLASS_NAME instance;
	return &instance;
}

CLASS_NAME::CLASS_NAME() : DmaUart(uart_cfg)
{

}

// Interrupt handlers ----------------------------------------------------------

/*extern "C" void DMA_RX_IRQ_HANDLER(void)
{
	CLASS_NAME::GetInstance()->HandleDmaRxIrq();
}*/

extern "C" void DMA_TX_IRQ_HANDLER(void)
{
	CLASS_NAME::GetInstance()->HandleDmaTxIrq();
}

extern "C" void UART_IRQ_HANDLER(void)
{
	CLASS_NAME::GetInstance()->HandleUartIrq();
}
