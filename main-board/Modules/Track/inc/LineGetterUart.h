#pragma once

#include "DmaUart.h"

class LineGetterUart : public DmaUart
{
public:
	virtual int32_t Receive(void* buffer, size_t& size, size_t targetSize) override;

protected:
	explicit LineGetterUart(DMA_UART_CFG& cfg);
};
