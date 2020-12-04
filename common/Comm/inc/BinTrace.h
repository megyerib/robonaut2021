#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "message_buffer.h" /* FreeRTOS */
#include "BinaryEncoder.h"
#include <memory>

#define TRACE_BIN(buf, size) BinTrace::GetInstance().TraceBinary(false, (buf), (size))

/*  ________          _______           ________
 * |        |        |  OS   | Coding  |  UART  |
 * | Caller |------->| Queue |-------->| Buffer |----> MAIN
 * |________|        |_______|         |________|               */

class BinTrace
{
public:
    static BinTrace& GetInstance();
    void TraceBinary(bool from_isr, const void* buf, size_t size);
    void Process();

private:
    MessageBufferHandle_t binMsgBuf;

    uint8_t* txBuf;
    size_t   txBufLen;

    BinTrace();
};
