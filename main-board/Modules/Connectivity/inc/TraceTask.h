#pragma once

#include <CmdReceiver.h>
#include "CyclicTask.h"
#include "Transmitter.h"
#include "Receiver.h"
#include "EscapeEncoder.h"

#define RX_TMP_BUF_LEN   (200u)

class TraceTask : public CyclicTask
{
public:
	static TraceTask& Init();

private:
	EscapeEncoder   enc;
	CommandReceiver crx;
	static uint8_t tmpBuf[RX_TMP_BUF_LEN];

	TraceTask();
	void TaskInit() override;
	void TaskFunction() override;

	void RxProcess();
};
