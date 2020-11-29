#pragma once

#include "ShiftReg.h"
#include "Line.h"


class Display
{
	ShiftReg leds;

public:
	Display();
	// Line structure -> Lighting LEDs on the sensor
	void DisplayLinePos(LineInput l);
	void DisplayPattern(uint32_t pattern);
	void Clear();

private:
	uint8_t mmToLedPos(int16_t mm);
	uint32_t transformReg_Rev1(uint32_t ledval);
};
