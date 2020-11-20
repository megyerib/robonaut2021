#include <CmdReceiver.h>
#include <cstring>
#include "crc8.h"

CommandReceiver::CommandReceiver(
		Receiver& receiver,
		uint8_t* tmpBuf,
		size_t tmpBufLen,
		BinaryEncoder* encoder)
		:
		receiver(receiver),
		tmpBuf(tmpBuf),
		tmpBufLen(tmpBufLen),
		encoder(encoder)
{

}

int32_t CommandReceiver::Receive(void* buffer, size_t& size, size_t targetSize)
{
	size = 0;

	// Receive raw data
	size_t receivedSize;
	receiver.Receive(&tmpBuf[tmpBufLast], receivedSize, (tmpBufLen - tmpBufLast));
	tmpBufLast += receivedSize;

	// If full, copy data to the beginning
	if (tmpBufLast == tmpBufLen)
	{
		size_t dataSize = tmpBufLast - tmpBufFirst;
		memmove(tmpBuf, &tmpBuf[tmpBufFirst], dataSize); // Memmove handles overlapping correctly (but it is slower)
		tmpBufFirst = 0;
		tmpBufLast  = dataSize;

		receiver.Receive(&tmpBuf[tmpBufLast], receivedSize, (tmpBufLen - tmpBufLast));
		tmpBufLast += receivedSize;
	}

	// Process raw data

	// Loop through leading \ns
	while ((tmpBuf[tmpBufFirst] == '\n') && (tmpBufFirst < tmpBufLast))
	{
		tmpBufFirst++;
	}

	size_t encFrameSize  = 0;

	while (tmpBuf[tmpBufFirst + encFrameSize] != '\n' && (tmpBufFirst + encFrameSize < tmpBufLast))
	{
		encFrameSize++;
	}

	if ((tmpBufFirst + encFrameSize < tmpBufLast) && (tmpBuf[tmpBufFirst + encFrameSize] == '\n'))
	{
		encoder->Decode(&tmpBuf[tmpBufFirst], encFrameSize, (uint8_t*)buffer, size);

		tmpBufFirst += encFrameSize + 1;

		if (Crc8::CheckBlockCrc(buffer, size))
		{
			size--;
		}
		else
		{
			size = 0;
		}
	}

	return RECEIVE_OK;
}
