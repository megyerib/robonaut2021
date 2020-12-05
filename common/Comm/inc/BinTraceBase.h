#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "message_buffer.h" /* FreeRTOS */
#include "BinaryEncoder.h"
#include "Transmitter.h"

/* Caller
 *   |
 *   V
 * OS Queue
 *   |  Coding
 *   V
 * UART Buffer
 *   |
 *   V
 * Main board
 */

class BinTraceBase
{
public:
	BinTraceBase(Transmitter& tx, uint8_t* txBuf, size_t txBufSize, size_t binQSize, BinaryEncoder& enc);

    void TraceBinary(bool from_isr, const void* buf, size_t size);
    void Process();

private:
    MessageBufferHandle_t binMsgBuf;

    Transmitter&   tx;
    uint8_t*       txBuf;
    size_t         txBufLen;
    size_t         txBufMax;
    BinaryEncoder& enc;
};
