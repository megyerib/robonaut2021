#pragma once

#include "CyclicTask.h"
#include <cstdint>
#include "CmdReceiver.h"
#include "EscapeEncoder.h"

#define CMD_PROC_BUF_LEN  (50u)

// Standalone task to run the UI board with the 2020 app.

class UiTask : CyclicTask
{
public:
	static UiTask& Init();

private:
	UiTask();

	void TaskInit() override;
	void TaskFunction() override;

	EscapeEncoder enc;
	CmdReceiver cmdProc;

	uint8_t cmdProcBuf[CMD_PROC_BUF_LEN];

	int  num = 0;
	bool manSetEnabled = false; // Manual setting enabled (is flashing?)
	static uint8_t txBuf;

	void ProcessRxQueue();
	void ProcessRxMessage(uint8_t buf[], size_t size);
};
