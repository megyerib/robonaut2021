#pragma once

#include "DmaUartF0.h"

class MainUart : public DmaUartF0
{
public:
	static MainUart& GetInstance();

private:
	MainUart();
};
