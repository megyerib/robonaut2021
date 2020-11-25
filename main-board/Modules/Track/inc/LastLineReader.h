#pragma once

#include "DmaUart.h"
#include "Receiver.h"

// TODO fancy design pattern!!!

class LastLineReader : public Receiver
{
public:
	LastLineReader(DmaUart* uart, size_t rxBufSize);
	~LastLineReader();

	virtual int32_t Receive(void* buffer, size_t& size, size_t targetSize) override;

private:
	DmaUart* uart;
	size_t rxBufSize;

	uint8_t* rxBuf;
	size_t rxBufIndex = 0;
};
