#pragma once

#include "DmaUart.h"

class MotorUart : public DmaUart
{
public:
	static MotorUart* GetInstance();

private:
	MotorUart();
};
