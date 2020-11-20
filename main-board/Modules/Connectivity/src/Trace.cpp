#include <TraceUart.h>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "EscapeEncoder.h"
#include "crc8.h"
#include "../inc/Trace.h"

#define MALLOC_PORT(x)            malloc(x)
#define FREE_PORT(x)              free(x)
#define TEXT_QUEUE_SIZE           32u
#define BIN_QUEUE_SIZE            64u

uint8_t Trace::txBuffers[2][TX_BUF_LEN];
size_t Trace::txBufLens[2];

static uint8_t uncodedBuf[200];

Trace& Trace::GetInstance()
{
    static Trace instance;
    return instance;
}

Trace::Trace()
{
	txBufNum = 0;
	txBuf    = (uint8_t*) &txBuffers[txBufNum];
	txBufLen = &txBufLens[txBufNum];

	txBufLens[0] = 0;
	txBufLens[1] = 0;

	// Init queues
	textMsgBuf = xMessageBufferCreate(TEXT_QUEUE_SIZE * sizeof(void*));
	binMsgBuf  = xMessageBufferCreate(BIN_QUEUE_SIZE * sizeof(void*));

	// Init encoder
	encoder.reset(new EscapeEncoder);
}

void Trace::Printf(bool from_isr, const char* format, ...)
{
    // Extract arguments
	va_list args;
    va_start(args, format); // https://en.cppreference.com/w/cpp/utility/variadic/va_start (format: preceding parameter)

    uint32_t arg_list[TRACE_MAX_ARGS];
    uint32_t arg_num = getPrintfTokenNum(format);
             arg_num = arg_num <= TRACE_MAX_ARGS ? arg_num : TRACE_MAX_ARGS;

    for (uint32_t i = 0; i < arg_num; i++)
    {
        arg_list[i] = va_arg(args, uint32_t);
    }

    va_end(args);

    // Prepare buffer
    size_t formatLen = strlen(format) + 1;
    size_t bufSize = sizeof(uint32_t) * (arg_num + 1) + formatLen;
    void* buffer = MALLOC_PORT(bufSize);

    *(uint32_t*)buffer = arg_num;
    memcpy((uint32_t*)buffer + 1, arg_list, arg_num * sizeof(uint32_t));
    memcpy((uint32_t*)buffer + 1 + arg_num, format, formatLen);

    // Push to queue
    if (!from_isr) // From task
    {
    	__disable_irq();
    	xMessageBufferSend(textMsgBuf, &buffer, sizeof(buffer), 0 /*Ticks to wait*/);
    	__enable_irq();
    }
    else // From ISR
    {
    	__disable_irq();
    	xMessageBufferSendFromISR(textMsgBuf, &buffer, sizeof(buffer), pdFALSE /*Don't notify any tasks*/);
    	__enable_irq();
    }
}

void Trace::PrintBinary(bool from_isr, const void* buf, size_t size)
{
	BINARY_PACKET* buffer = (BINARY_PACKET*) MALLOC_PORT(sizeof(BINARY_PACKET) + size);

	buffer->len = size;
	memcpy(buffer->data, buf, size);

	// Push to queue
	if (!from_isr) // From task
	{
		__disable_irq();
		xMessageBufferSend(binMsgBuf, &buffer, sizeof(buffer), 0 /*Ticks to wait*/);
		__enable_irq();
	}
	else // From ISR
	{
		__disable_irq();
		xMessageBufferSendFromISR(binMsgBuf, &buffer, sizeof(buffer), pdFALSE /*Don't notify any tasks*/);
		__enable_irq();
	}
}

uint32_t Trace::getPrintfTokenNum(const char* format)
{
    bool prev_percent = false;
    uint32_t tokens = 0;

    for (; *format != '\0'; format++)
    {
        if (!prev_percent)
        {
            if (*format == '%')
            {
                prev_percent = true;
            }
        }
        else
        {
            if (*format != '%')
            {
                tokens++;
            }
            prev_percent = false;
        }
    }

    return tokens;
}

void Trace::ProcessBinary()
{
	while (true)
	{
		BINARY_PACKET* packet;

		size_t ptrSize = xMessageBufferReceive(binMsgBuf, &packet, sizeof(packet), 0 /*Ticks to wait*/);

		if (ptrSize == sizeof(packet))
		{
			// Copy to tmp buffer
			size_t size = packet->len;
			memcpy(uncodedBuf, packet->data, size);

			// CRC
			uncodedBuf[size] = Crc8::GetBlockCrc(uncodedBuf, size);
			size++;

			// Code & copy to Tx queue
			EscapeEncoder enc;
			size_t encSize;
			enc.Encode(uncodedBuf, size, &txBuf[*txBufLen], encSize);
			*txBufLen += encSize;

			// Line ending
			txBuf[*txBufLen] = '\n';
			(*txBufLen)++;

			// Free buffer
			FREE_PORT(packet);
		}
		else if (ptrSize == 0)
		{
			break;
		}
		else
		{
			// Error
		}
	}
}

void Trace::ProcessText()
{
	while (true)
	{
		void* printfBuf;

		size_t ptrSize = xMessageBufferReceive(textMsgBuf, &printfBuf, sizeof(printfBuf), 0 /*Ticks to wait*/);

		if (ptrSize == sizeof(void*))
		{
			// Print
			uint32_t argNum  = *(uint32_t*)printfBuf;
			uint32_t* args   = (uint32_t*)printfBuf + 1;
			char* format     = (char*) &args[argNum];

			size_t uncLen = 0;

			// Insert leading zero (text message code)
			uncodedBuf[uncLen] = 0;
			uncLen++;

			switch (argNum)
			{
				case 0:
				{
					// It would be better if we simply copied everything. BUT. Strncpy can't retrieve
					// the # of characters copied into the destination buffer. (plus the terminating
					// zero isn't required either)
					uncLen += snprintf((char*)&uncodedBuf[uncLen], TRACE_MAX_STRLEN, format);
					break;
				}
				case 1:
				{
					uncLen += snprintf((char*)&uncodedBuf[uncLen], TRACE_MAX_STRLEN, format, args[0]);
					break;
				}
				case 2:
				{
					uncLen += snprintf((char*)&uncodedBuf[uncLen], TRACE_MAX_STRLEN, format, args[0], args[1]);
					break;
				}
				default:
					break;
			}

			// CRC
			uncodedBuf[uncLen] = Crc8::GetBlockCrc(uncodedBuf, uncLen);
			uncLen++;

			// Code & copy to Tx queue
			EscapeEncoder enc;
			size_t encSize;
			enc.Encode(uncodedBuf, uncLen, &txBuf[*txBufLen], encSize);
			*txBufLen += encSize;

			// Line ending
			txBuf[*txBufLen] = '\n';
			(*txBufLen)++;

			// Free buffer
			FREE_PORT(printfBuf);
		}
		else if (ptrSize == 0)
		{
			break;
		}
		else
		{
			// Error
		}
	}
}

void Trace::Process()
{
	if (*txBufLen > 0)
	{
		// Send buffer, switch to the another one
		TraceUart::GetInstance().Transmit(txBuf, *txBufLen);

		txBufNum ^= 1; // 0 <-> 1
		txBuf = (uint8_t*) &txBuffers[txBufNum];
		txBufLen = &txBufLens[txBufNum];

		*txBufLen = 0;
	}

	// Process binary messages
	ProcessBinary();

	// Process text messages
	ProcessText();
}
