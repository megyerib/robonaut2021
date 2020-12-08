#pragma once

#include "DmaUartF4.h"

class LsFrontUart : public DmaUartF4
{
public:
	static LsFrontUart& GetInstance();

private:
	LsFrontUart();
};
