#pragma once

#include "ShiftReg.h"

class IrDriver
{
	ShiftReg ir;

public:
	static IrDriver& GetInstance();
	void SetSensors(uint8_t groupSize, uint8_t index);
	void DisplayPattern(uint32_t pattern);
	void Enable(bool en);

private:
	IrDriver();
	uint32_t transformReg_Rev1(uint32_t irval);
};
