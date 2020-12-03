#pragma once

#include <DmaUartF4.h>

class TraceUart : public DmaUartF4
{
public:
	static TraceUart& GetInstance();

private:
	TraceUart();
};
