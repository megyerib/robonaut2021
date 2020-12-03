#pragma once

#include <DmaUartF4.h>

class MotorUart : public DmaUartF4
{
public:
	static MotorUart& GetInstance();

private:
	MotorUart();
};
