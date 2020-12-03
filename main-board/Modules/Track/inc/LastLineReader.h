#pragma once

#include <DmaUartF4.h>
#include "Receiver.h"

class LastLineReader : public Receiver
{
public:
	LastLineReader(DmaUartF4* uart, size_t rxBufSize);
	~LastLineReader();

	virtual int32_t Receive(void* buffer, size_t& size, size_t targetSize) override;

private:
	DmaUartF4* uart;
	size_t rxBufSize;

	uint8_t* rxBuf;
	size_t rxBufIndex = 0;
};
