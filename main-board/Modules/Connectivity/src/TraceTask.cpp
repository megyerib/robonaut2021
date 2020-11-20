#include "TraceTask.h"

#include "../inc/Trace.h"
#include "TraceUart.h"

#define PRIO       5
#define PERIOD    10
#define STACK    512

uint8_t TraceTask::tmpBuf[RX_TMP_BUF_LEN];

TraceTask::TraceTask() : CyclicTask((char*)"Trace", PERIOD, PRIO, STACK),
                         crx(TraceUart::GetInstance(), tmpBuf, RX_TMP_BUF_LEN, &enc)

{

}

TraceTask& TraceTask::Init()
{
	static TraceTask instance;
	return instance;
}

void TraceTask::TaskInit()
{
	TRACE_INIT();
}

void TraceTask::TaskFunction()
{
	Trace::GetInstance().Process();
	RxProcess();
}

void TraceTask::RxProcess()
{
	uint8_t buf[20];
	size_t  size = 0;

	while(true)
	{
		crx.Receive(buf, size, 20);

		if (size != 0)
		{
			// Process frame here
			TRACE_BIN(buf, size); // Echo it back for example
		}
		else
		{
			break;
		}
	}
}
