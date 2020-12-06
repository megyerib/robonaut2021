#pragma once

#include "ShiftReg.h"
#include "Line.h"

// TODO interface is OK but the whole module needs refactoring

class LedDriver
{
	ShiftReg leds;

public:
	static LedDriver& GetInstance();
	void DisplayLinePos(LineInput l);
	void DisplayPattern(uint32_t pattern);
	void Clear();

private:
	LedDriver();

	uint8_t mmToLedPos(int16_t mm);
	uint32_t transformReg_Rev1(uint32_t ledval);
};
