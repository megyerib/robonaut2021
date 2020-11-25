#include "LineGetterUart.h"
#include <cstring>

LineGetterUart::LineGetterUart(DMA_UART_CFG& cfg) : DmaUart(cfg)
{

}

int32_t LineGetterUart::Receive(void* buffer, size_t& size, size_t targetSize)
{
	UNUSED(targetSize); // TODO

	uint8_t* buf = (uint8_t*) buffer;
	size_t bufferEnd;
	size_t end = rxBufIndex;

	size = 0;

	bufferEnd = cfg.rxBufSize - hdma_uart_rx.Instance->NDTR;

	// DETERMINE LINE END

	for (size_t i = rxBufIndex; i != bufferEnd;)
	{
		if (cfg.rxBuf[i] == '\n')
		{
			end = i;
			break;
		}

		i++;
		i %= cfg.rxBufSize;
	}

	// COPY
	
	if (rxBufIndex <= end)
	{
		size = end - rxBufIndex;
		memcpy(buf, &cfg.rxBuf[rxBufIndex], size);
	}
	else // Circular copy
	{
		int size1 = cfg.rxBufSize - rxBufIndex;
		int size2 = end;

		memcpy(buf, &cfg.rxBuf[rxBufIndex], size1);
		memcpy(&buf[size1], cfg.rxBuf, size2);

		size = size1 + size2;
	}

	rxBufIndex = end;

	if (cfg.rxBuf[rxBufIndex] == '\n')
	{
		rxBufIndex++;
		rxBufIndex %= cfg.rxBufSize;
	}

	return RECEIVE_OK;
}
