#pragma once

#include "BinTraceBase.h"

#define MSG_BUF_SIZE  (32u)
#define TX_BUF_SIZE   (32u)

#define TRACE_BIN(buf, size) BinTrace::GetInstance().TraceBinary(false, (buf), (size))

class BinTrace : public BinTraceBase
{
public:
	static BinTrace& GetInstance();

private:
	static uint8_t txBuf[TX_BUF_SIZE];

	BinTrace();
};
