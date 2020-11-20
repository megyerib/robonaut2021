#pragma once

#include "DmaUart.h"

class TraceUart : public DmaUart
{
public:
	static TraceUart& GetInstance();

private:
	TraceUart();
};
