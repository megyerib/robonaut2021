#pragma once

#include "CyclicTask.h"
#include <cstdint>

// Standalone task to run the UI board with the 2020 app.

class UiTask : CyclicTask
{
public:
	static UiTask& Init();

private:
	UiTask();

	void TaskInit() override;
	void TaskFunction() override;

	int  num = 0;
	bool blinking = false;
	int cntr = 0;
	bool showNum = true;
	static uint8_t txBuf;
	uint8_t rxBuf = 0; // Some value had to be assigned to suppress warning
};
