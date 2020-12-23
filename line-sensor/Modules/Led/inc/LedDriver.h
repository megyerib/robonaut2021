#pragma once

#include "ShiftReg.h"
#include "Line.h"

class LedDriver
{
	ShiftReg leds;

public:
	static LedDriver& GetInstance();
	void DisplayLinePos(LineInput l);
	void DisplayPattern(uint32_t pattern);
	void Clear();
	void Enable(bool en);

private:
	LedDriver();

	uint8_t unitToLedPos(int16_t unit);
	uint32_t transformReg_Rev1(uint32_t ledval);
};
