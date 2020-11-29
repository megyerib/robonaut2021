#pragma once

#include "ShiftReg.h"

class SensorDriver
{
	ShiftReg sensors;

public:
	SensorDriver();
	void SetSensors(uint8_t groupSize, uint8_t index);
	void DisplayPattern(uint32_t pattern);

private:
	uint32_t transformReg_Rev1(uint32_t irval);
};
