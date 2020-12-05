#include "BinTrace.h"
#include "MainUart.h"
#include "EscapeEncoder.h"


uint8_t BinTrace::txBuf[TX_BUF_SIZE];
static EscapeEncoder escEnc;

BinTrace::BinTrace() : BinTraceBase(MainUart::GetInstance(),
		                            txBuf,
									TX_BUF_SIZE,
									MSG_BUF_SIZE,
									escEnc)
{

}

BinTrace& BinTrace::GetInstance()
{
	static BinTrace instance;
	return instance;
}
