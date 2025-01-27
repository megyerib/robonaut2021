#include "BinTraceBase.h"
#include "crc8.h"
#include "cmsis_gcc.h" /* EI/DI; uC-independent (STM32 family only) */

BinTraceBase::BinTraceBase(Transmitter& tx, uint8_t* txBuf, size_t txBufSize, size_t binQSize, BinaryEncoder& enc) : tx(tx), enc(enc)
{
	binMsgBuf       = xMessageBufferCreate(binQSize * sizeof(void*));
	this->txBuf     = txBuf;
	this->txBufLen  = 0;
	this->txBufMax  = txBufSize;
}

void BinTraceBase::TraceBinary(bool from_isr, const void* buf, size_t size)
{
	if (size > 0)
	{
		if (!from_isr) // Push to queue from task
		{
			__disable_irq();
			xMessageBufferSend(binMsgBuf, buf, size, 0 /*Ticks to wait*/);
			__enable_irq();
		}
		else // Push to queue from ISR
		{
			__disable_irq();
			xMessageBufferSendFromISR(binMsgBuf, buf, size, pdFALSE /*Don't notify any tasks*/);
			__enable_irq();
		}
	}
}

void BinTraceBase::Process()
{
	static uint8_t tmpBuf[66]; // 66: Length of the largest possible message
	// For example: Buffer the data was created in -> RTOS Stream buffer -> Tx handler decoding buffer ->
	// -> UART Tx buffer ...

	while (true)
	{
		size_t size = xMessageBufferReceive(binMsgBuf, tmpBuf, sizeof(tmpBuf), 0 /*Ticks to wait*/);

		if (size > 0)
		{
			// CRC
			tmpBuf[size] = Crc8::GetBlockCrc(tmpBuf, size);
			size++;

			// Code & copy to Tx queue
			size_t encSize;
			enc.Encode(tmpBuf, size, &txBuf[txBufLen], encSize, txBufMax - txBufLen);
			txBufLen += encSize;

			// Line ending
			txBuf[txBufLen] = '\n';
			txBufLen++;
		}
		else
		{
			break;
		}
	}

	if (txBufLen > 0)
	{
		tx.Transmit(txBuf, txBufLen);
		txBufLen = 0;
	}
}
