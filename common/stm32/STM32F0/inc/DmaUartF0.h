#pragma once

#include "stm32f0xx_hal.h"
#include "Transmitter.h"
#include "Receiver.h"

typedef void(*ClkEn)(void); // Function pointer to lambda function

typedef struct
{
	// Tx ------------------------------
	bool                 txEnabled;

	// Rx ------------------------------
	bool                 rxEnabled;
	uint8_t*             rxBuf;
	size_t               rxBufSize;

	// GPIO ----------------------------
	ClkEn                gpioTxClkEn;
	GPIO_TypeDef*        gpioTxPort;
	uint16_t             gpioTxPin;

	ClkEn                gpioRxClkEn;
	GPIO_TypeDef*        gpioRxPort;
	uint16_t             gpioRxPin;

	uint8_t              gpioAf;

	// DMA -----------------------------
	ClkEn                dmaClkEn;

	DMA_Channel_TypeDef* dmaTxChannel;
	IRQn_Type            dmaTxIrq;
	uint32_t             dmaTxNvicPrio;

	DMA_Channel_TypeDef* dmaRxChannel;
	IRQn_Type            dmaRxIrq;
	uint32_t             dmaRxNvicPrio;

	// UART ----------------------------
	ClkEn                uartClkEn;
	USART_TypeDef*       uartInstance;
	uint32_t             uartBaudRate;
	IRQn_Type            uartIrq;
	uint32_t             uartNvicPrio;
	bool                 uartSwapPins;
}
DMA_UART_CFG;

class DmaUartF0 : public Transmitter, public Receiver
{
public:
	virtual int32_t Transmit(const void* buffer, size_t size) override;
	virtual int32_t Receive(void* buffer, size_t& size, size_t targetSize) override;

	void HandleUartIrq();
	void HandleDmaRxIrq();
	void HandleDmaTxIrq();

protected:
	explicit DmaUartF0(const DMA_UART_CFG& cfg);

	size_t rxBufIndex = 0;
	const DMA_UART_CFG& cfg;

	UART_HandleTypeDef huart;
	DMA_HandleTypeDef  hdma_uart_tx;
	DMA_HandleTypeDef  hdma_uart_rx;

	void InitUart();
	void InitGpio();
	void InitDma();

private:
	bool   txActive = false;
	size_t txRead   = 0;
	size_t txWrite  = 0;
	size_t txFinish = 0;

	// Copy & set index
	void copyToCircular(const void* src, size_t srcSize, void* buf, size_t& bufIndex, size_t bufSize);
};
