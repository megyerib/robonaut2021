#include "LsUartFront.h"
#include "NvicPrio.h"

#define RX_BUF_SIZE (100u)
#define UART_IRQ_HANDLER    USART3_IRQHandler
#define DMA_RX_IRQ_HANDLER  DMA1_Stream1_IRQHandler
#define DMA_TX_IRQ_HANDLER  DMA1_Stream3_IRQHandler
#define CLASS_NAME          LsUartFront

static uint8_t rxBuffer[RX_BUF_SIZE];

static DMA_UART_CFG uart_cfg =
{
	.txEnabled     = true,
	.rxEnabled     = true,

	// Rx buffer
	.rxBuf         = rxBuffer,
	.rxBufSize     = RX_BUF_SIZE,

	// GPIO
	.gpioTxClkEn   = [](){__HAL_RCC_GPIOC_CLK_ENABLE();},
	.gpioTxPort    = GPIOC,
	.gpioTxPin     = GPIO_PIN_10,

	.gpioRxClkEn   = [](){__HAL_RCC_GPIOC_CLK_ENABLE();},
	.gpioRxPort    = GPIOC,
	.gpioRxPin     = GPIO_PIN_11,

	.gpioAf        = GPIO_AF7_USART3,

	// DMA
	.dmaClkEn      = [](){__HAL_RCC_DMA1_CLK_ENABLE();},

	.dmaTxStream   = DMA1_Stream3,
	.dmaTxChannel  = DMA_CHANNEL_4,
	.dmaTxIrq      = DMA1_Stream3_IRQn,
	.dmaTxNvicPrio = DMA_NVIC_PRIO,

	.dmaRxStream   = DMA1_Stream1,
	.dmaRxChannel  = DMA_CHANNEL_4,
	.dmaRxIrq      = DMA1_Stream1_IRQn,
	.dmaRxNvicPrio = DMA_NVIC_PRIO,

	// UART
	.uartClkEn     = [](){__HAL_RCC_USART3_CLK_ENABLE();},
	.uartInstance  = USART3,
	.uartBaudRate  = 115200,
	.uartIrq       = USART3_IRQn,
	.uartNvicPrio  = DMA_NVIC_PRIO,
};

// No touching needed ----------------------------------------------------------

CLASS_NAME* CLASS_NAME::GetInstance()
{
	static CLASS_NAME instance;
	return (&instance);
}

CLASS_NAME::CLASS_NAME() : LineGetterUart(uart_cfg)
{

}

// Interrupt handlers ----------------------------------------------------------

extern "C" void DMA_RX_IRQ_HANDLER(void)
{
	CLASS_NAME::GetInstance()->HandleDmaRxIrq();
}

extern "C" void DMA_TX_IRQ_HANDLER(void)
{
	CLASS_NAME::GetInstance()->HandleDmaTxIrq();
}

extern "C" void UART_IRQ_HANDLER(void)
{
	CLASS_NAME::GetInstance()->HandleUartIrq();
}
