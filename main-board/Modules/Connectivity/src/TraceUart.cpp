#include "TraceUart.h"
#include "NvicPrio.h"

// Chosen UART device
#define ROBOT   /* USART6 is used */
//#define NUCLEO  /* USART2 is used */

// Common data
#define CLASS_NAME          TraceUart
#define RX_BUF_SIZE            (200u)
#define BAUDRATE           (1000000u)

static uint8_t rxBuffer[RX_BUF_SIZE];

#ifdef ROBOT /*-------------------------------------------------------------------------------------------------------*/

#define UART_IRQ_HANDLER    USART6_IRQHandler
#define DMA_TX_IRQ_HANDLER  DMA2_Stream6_IRQHandler
#define DMA_RX_IRQ_HANDLER  DMA2_Stream1_IRQHandler

static DMA_UART_CFG uart_cfg =
{
	// Tx
	.txEnabled     = true,

	// Rx
	.rxEnabled     = true,
	.rxBuf         = rxBuffer,
	.rxBufSize     = RX_BUF_SIZE,

	// GPIO
	.gpioTxClkEn   = [](){__HAL_RCC_GPIOC_CLK_ENABLE();},
	.gpioTxPort    = GPIOC,
	.gpioTxPin     = GPIO_PIN_6,

	.gpioRxClkEn   = [](){__HAL_RCC_GPIOC_CLK_ENABLE();},
	.gpioRxPort    = GPIOC,
	.gpioRxPin     = GPIO_PIN_7,

	.gpioAf        = GPIO_AF8_USART6,

	// DMA
	.dmaClkEn      = [](){__HAL_RCC_DMA2_CLK_ENABLE();},

	.dmaTxStream   = DMA2_Stream6,
	.dmaTxChannel  = DMA_CHANNEL_5,
	.dmaTxIrq      = DMA2_Stream6_IRQn,
	.dmaTxNvicPrio = DMA_NVIC_PRIO,

	.dmaRxStream   = DMA2_Stream1,
	.dmaRxChannel  = DMA_CHANNEL_5,
	.dmaRxIrq      = DMA2_Stream1_IRQn,
	.dmaRxNvicPrio = DMA_NVIC_PRIO,

	// UART
	.uartClkEn     = [](){__HAL_RCC_USART6_CLK_ENABLE();},
	.uartInstance  = USART6,
	.uartBaudRate  = BAUDRATE,
	.uartIrq       = USART6_IRQn,
	.uartNvicPrio  = TRACE_UART_NVIC_PRIO,
};

#endif

#ifdef NUCLEO /*------------------------------------------------------------------------------------------------------*/

#define UART_IRQ_HANDLER    USART2_IRQHandler
#define DMA_TX_IRQ_HANDLER  DMA1_Stream6_IRQHandler
#define DMA_RX_IRQ_HANDLER  DMA1_Stream5_IRQHandler

static DMA_UART_CFG uart_cfg =
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
	.uartBaudRate  = BAUDRATE,
	.uartIrq       = USART2_IRQn,
	.uartNvicPrio  = TRACE_UART_NVIC_PRIO,
};

#endif


// Don't touch ---------------------------------------------------------------------------------------------------------

CLASS_NAME& CLASS_NAME::GetInstance()
{
	static CLASS_NAME instance;
	return instance;
}

CLASS_NAME::CLASS_NAME() : DmaUartF4(uart_cfg)
{

}

// Interrupt handlers --------------------------------------------------------------------------------------------------

extern "C" void DMA_RX_IRQ_HANDLER(void)
{
	CLASS_NAME::GetInstance().HandleDmaRxIrq();
}

extern "C" void DMA_TX_IRQ_HANDLER(void)
{
	CLASS_NAME::GetInstance().HandleDmaTxIrq();
}

extern "C" void UART_IRQ_HANDLER(void)
{
	CLASS_NAME::GetInstance().HandleUartIrq();
}

// End -----------------------------------------------------------------------------------------------------------------
