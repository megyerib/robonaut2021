#include <LastLineReader.h>
#include <cstring>

LastLineReader::LastLineReader(DmaUart* uart, size_t rxBufSize)
	: uart(uart),
	  rxBufSize(rxBufSize)
{
	rxBuf = new uint8_t[rxBufSize];
}

LastLineReader::~LastLineReader()
{
	delete rxBuf;
}

int32_t LastLineReader::Receive(void* buffer, size_t& size, size_t targetSize)
{
	size_t received;

	uart->Receive(&rxBuf[rxBufIndex], received, rxBufSize - rxBufIndex);

	rxBufIndex += received;

	// Get last valid block ------------------------------------------

	size_t blockBegin = 0;
	size_t blockSize = 0;

	size_t validBegin = 0;
	size_t validSize = 0;

	for (size_t i = 0; i < rxBufIndex; i++)
	{
		if (rxBuf[i] == '\n')
		{
			validBegin = blockBegin;
			validSize  = blockSize;

			blockBegin = i + 1;
			blockSize = 0;
		}
		else
		{
			blockSize++;
		}
	}

	// Copy block ----------------------------------------------------

	if (validSize > 0)
	{
		memcpy(buffer, &rxBuf[validBegin], validSize);
		size = validSize;
	}
	else
	{
		size = 0;
	}

	// Tidy up -------------------------------------------------------

	if (rxBuf[rxBufIndex - 1] == '\n')
	{
		rxBufIndex = 0;
	}
	else
	{
		if (blockSize <= blockBegin)
		{
			memcpy(rxBuf, &rxBuf[blockBegin], blockSize);
		}
		else
		{
			uint8_t* tmpBuf = new uint8_t[blockSize];

			memcpy(tmpBuf, &rxBuf[blockBegin], blockSize);
			memcpy(rxBuf, tmpBuf, blockSize);

			delete tmpBuf;
		}

		rxBufIndex = blockSize;
	}

	return RECEIVE_OK;
}
