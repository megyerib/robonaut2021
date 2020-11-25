#pragma once

#include "DmaUart.h"
#include "LineGetterUart.h"

class LsUartRear : public LineGetterUart
{
public:
	static LsUartRear* GetInstance();

private:
	LsUartRear();
};
