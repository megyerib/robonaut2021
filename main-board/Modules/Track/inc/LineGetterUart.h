#pragma once

#include <DmaUartF4.h>

class LineGetterUart : public DmaUartF4
{
public:
	virtual int32_t Receive(void* buffer, size_t& size, size_t targetSize) override;

protected:
	explicit LineGetterUart(DMA_UART_CFG& cfg);
};
