#pragma once

#include "CyclicTask.h"
#include <cstdint>
#include "CmdReceiver.h"
#include "EscapeEncoder.h"
#include "SensorCfg.h"

#define CMD_PROC_BUF_LEN 20u

class Line2020Task : CyclicTask
{
public:
	static Line2020Task& Init();

private:
	Line2020Task();

	void TaskInit() override;
	void TaskFunction() override;

	void ProcessRxQueue();
	void ProcessRxMessage(uint8_t buf[], size_t size);
	void TraceSensorData(uint32_t data[SENSOR_SIZE]);

	EscapeEncoder enc;
	CmdReceiver cmdProc;
	uint8_t cmdProcBuf[CMD_PROC_BUF_LEN];

	bool lineDataEn   = true;
	bool ledEn        = true;
	bool measEn       = true; // Effectively: IR LEDs enable
	bool sensorDataEn = false;
};
