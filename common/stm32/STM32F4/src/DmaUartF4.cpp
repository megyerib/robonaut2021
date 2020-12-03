#include <DmaUartF4.h>
#include <string.h>

DmaUartF4::DmaUartF4(DMA_UART_CFG& cfg) : cfg(cfg)
{
	InitGpio();
	InitUart();
	InitDma();

	HAL_UART_Receive_DMA(&huart, cfg.rxBuf, cfg.rxBufSize);
}

int32_t DmaUartF4::Transmit(const void* buffer, size_t size)
{
	HAL_UART_Transmit_DMA(&huart, (uint8_t*)buffer, size);

	return TRANSMIT_OK;
}

int32_t DmaUartF4::Receive(void* buffer, size_t& size, size_t targetSize)
{
	size_t end = cfg.rxBufSize - hdma_uart_rx.Instance->NDTR;

	if (rxBufIndex <= end)
	{
		// Calculating buffer size
		size = end - rxBufIndex;

		if (size > targetSize)
		{
			size = targetSize;
		}

		// Copy buffer
		memcpy(buffer, &cfg.rxBuf[rxBufIndex], size);
	}
	else // Circular copy
	{
		// Calculating buffer sizes
		size_t size1 = cfg.rxBufSize - rxBufIndex;
		size_t size2 = end;

		if (size1 > targetSize)
		{
			size1 = targetSize;
			size2 = 0;
		}

		if (size2 > (targetSize - size1))
		{
			size2 = (targetSize - size1);
		}

		size = size1 + size2;

		// Copy buffers
		memcpy(buffer, &cfg.rxBuf[rxBufIndex], size1);
		memcpy(((uint8_t*)buffer + size1), cfg.rxBuf, size2);
	}

	rxBufIndex += size;
	rxBufIndex %= cfg.rxBufSize;

	return RECEIVE_OK;
}

void DmaUartF4::copyToCircular(const void* src, size_t srcSize, void* buf, size_t& bufIndex, size_t bufSize)
{
	if ((bufIndex + srcSize) < bufSize)
	{
		memcpy(((uint8_t*)buf + bufIndex), src, srcSize);
	}
	else
	{
		size_t size1 = bufSize - bufIndex;
		size_t size2 = srcSize - size1;

		memcpy(((uint8_t*)buf + bufIndex), src, size1);
		memcpy(buf, ((uint8_t*)src + size1), size2);
	}

	bufIndex += srcSize;
	bufIndex %= bufSize;
}

void DmaUartF4::HandleUartIrq()
{
	HAL_UART_IRQHandler(&huart);
}

void DmaUartF4::HandleDmaRxIrq()
{
	HAL_DMA_IRQHandler(&hdma_uart_rx);
}

void DmaUartF4::HandleDmaTxIrq()
{
	HAL_DMA_IRQHandler(&hdma_uart_tx);
}

void DmaUartF4::InitUart()
{
	cfg.uartClkEn();

	huart.Instance          = cfg.uartInstance;
	huart.Init.BaudRate     = cfg.uartBaudRate;
	huart.Init.WordLength   = UART_WORDLENGTH_8B;
	huart.Init.StopBits     = UART_STOPBITS_1;
	huart.Init.Parity       = UART_PARITY_NONE;
	huart.Init.Mode         = UART_MODE_TX_RX;
	huart.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	huart.Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(&huart);

	// Interrupts
	HAL_NVIC_SetPriority(cfg.uartIrq, cfg.uartNvicPrio, 0);
	HAL_NVIC_EnableIRQ(cfg.uartIrq);

	// Disable unnecessary interrupts
	CLEAR_BIT(huart.Instance->CR1, (USART_CR1_TXEIE | USART_CR1_RXNEIE));
	// Transfer complete interrupt is necessary
}

void DmaUartF4::InitGpio()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin       = cfg.gpioTxPin;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = cfg.gpioAf;

	if (cfg.txEnabled)
	{
		cfg.gpioTxClkEn();
		HAL_GPIO_Init(cfg.gpioTxPort, &GPIO_InitStruct);
	}

	if (cfg.rxEnabled)
	{
		GPIO_InitStruct.Pin = cfg.gpioRxPin;

		cfg.gpioRxClkEn();
		HAL_GPIO_Init(cfg.gpioRxPort, &GPIO_InitStruct);
	}
}

void DmaUartF4::InitDma()
{
	cfg.dmaClkEn();

	// DMA Tx stream
	if (cfg.txEnabled)
	{
		hdma_uart_tx.Instance                 = cfg.dmaTxStream;
		hdma_uart_tx.Init.Channel             = cfg.dmaTxChannel;
		hdma_uart_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
		hdma_uart_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
		hdma_uart_tx.Init.MemInc              = DMA_MINC_ENABLE;
		hdma_uart_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_uart_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		hdma_uart_tx.Init.Mode                = DMA_NORMAL;
		hdma_uart_tx.Init.Priority            = DMA_PRIORITY_LOW;
		hdma_uart_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;

		HAL_DMA_Init(&hdma_uart_tx);
		__HAL_LINKDMA(&huart, hdmatx, hdma_uart_tx);

		// Set other registers
		//CLEAR_BIT(hdma_uart_tx.Instance->CR, DMA_SxCR_HTIE); // Disable half transfer interrupt

		HAL_NVIC_SetPriority(cfg.dmaTxIrq, cfg.dmaTxNvicPrio, 0);
		HAL_NVIC_EnableIRQ(cfg.dmaTxIrq);
	}

	// DMA Rx stream
	if (cfg.rxEnabled)
	{
		hdma_uart_rx.Instance                 = cfg.dmaRxStream;
		hdma_uart_rx.Init.Channel             = cfg.dmaRxChannel;
		hdma_uart_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		hdma_uart_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
		hdma_uart_rx.Init.MemInc              = DMA_MINC_ENABLE;
		hdma_uart_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_uart_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		hdma_uart_rx.Init.Mode                = DMA_CIRCULAR;
		hdma_uart_rx.Init.Priority            = DMA_PRIORITY_LOW;
		hdma_uart_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;

		HAL_DMA_Init(&hdma_uart_rx);
		__HAL_LINKDMA(&huart, hdmarx, hdma_uart_rx);

		HAL_NVIC_SetPriority(cfg.dmaRxIrq, cfg.dmaRxNvicPrio, 0);
		HAL_NVIC_EnableIRQ(cfg.dmaRxIrq);
	}
}
