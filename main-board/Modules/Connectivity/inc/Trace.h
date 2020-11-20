#pragma once
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "message_buffer.h"
#include "BinaryEncoder.h"
#include <memory>

#define TX_BUF_LEN   1000u

// Max. 32 bites int (char is) vagy const char* lehet paraméter, max. 4 paraméter
#define TRACE_INIT()    (void) Trace::GetInstance()
#define PRINTF(...)     Trace::GetInstance().Printf(false, __VA_ARGS__)
#define PRINTF_ISR(...) Trace::GetInstance().Printf(true, __VA_ARGS__)
#define TRACE_BIN(data, size) Trace::GetInstance().PrintBinary(false, data, size)

#define TRACE_MAX_STRLEN  (200u)
#define TRACE_MAX_ARGS    (4u)

typedef struct
{
	size_t  len;
	uint8_t data[];
}
BINARY_PACKET;

class Trace
{
public:
    static Trace& GetInstance();
    void Printf(bool from_isr, const char* format, ...);
    void PrintBinary(bool from_isr, const void* buf, size_t size);
    void Process();

private:
    static uint8_t txBuffers[2][TX_BUF_LEN];
    static size_t txBufLens[2];

    // Actually used buffers
    uint8_t txBufNum;
    uint8_t* txBuf;
    size_t* txBufLen;

    std::unique_ptr<BinaryEncoder> encoder;

    MessageBufferHandle_t textMsgBuf;
    MessageBufferHandle_t binMsgBuf;

    Trace();

    uint32_t getPrintfTokenNum(const char* format);

    void ProcessBinary();
    void ProcessText();
};
