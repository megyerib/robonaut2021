#pragma once

#include "DmaUartF4.h"

class LsRearUart : public DmaUartF4
{
public:
	static LsRearUart& GetInstance();

private:
	LsRearUart();
};
