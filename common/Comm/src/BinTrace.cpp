#include "BinTrace.h"
#include "EscapeEncoder.h"
#include "crc8.h"
#include "stm32f0xx_hal.h"
#include "MainUart.h"

#define BIN_QUEUE_SIZE (50u)
#define TX_BUF_LEN   (50u) // TODO calc size & baudrate
static uint8_t transmitBuf[TX_BUF_LEN];

BinTrace::BinTrace()
{
	binMsgBuf = xMessageBufferCreate(BIN_QUEUE_SIZE * sizeof(void*));
	txBuf     = transmitBuf;
	txBufLen  = 0;
}

BinTrace& BinTrace::GetInstance()
{
	static BinTrace instance;
	return instance;
}

void BinTrace::TraceBinary(bool from_isr, const void* buf, size_t size)
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

void BinTrace::Process()
{
	uint8_t tmpBuf[10];

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
			EscapeEncoder enc;
			enc.Encode(tmpBuf, size, &txBuf[txBufLen], encSize);
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
		MainUart::GetInstance().Transmit(txBuf, txBufLen);
		txBufLen = 0;
	}
}
