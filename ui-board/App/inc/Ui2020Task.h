#pragma once

#include "CyclicTask.h"
#include <cstdint>

#define PRIO                 2
#define PERIOD               1
#define STACK              128

class Ui2020Task : CyclicTask
{
public:
	static Ui2020Task& Init();

private:
	Ui2020Task();

	void TaskInit() override;
	void TaskFunction() override;

	int  num = 0;
	bool blinking = false;
	int cntr = 0;
	bool showNum = true;
	static uint8_t txBuf;
	uint8_t rxBuf = 0; // Some value had to be assigned to suppress warning
};
