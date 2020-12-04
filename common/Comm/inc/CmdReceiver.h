#pragma once

#include "Receiver.h"
#include "BinaryEncoder.h"

// This is a class which can receive binary frames from a serial line.
// When instantiated it must be inded with a Receiver, a temporary buffer (which shall be provided by the user) and
// a BinaryEncoder algorithm/class.

class CmdReceiver : public Receiver
{
public:
	CmdReceiver(Receiver& receiver, uint8_t* tmpBuf, size_t tmpBufLen, BinaryEncoder* encoder);
	virtual int32_t Receive(void* buffer, size_t& size, size_t targetSize) override;

private:
	Receiver&            receiver;
	uint8_t* const       tmpBuf;
	const size_t         tmpBufLen;
	BinaryEncoder* const encoder;

	size_t tmpBufFirst = 0;
	size_t tmpBufLast  = 0;
};
