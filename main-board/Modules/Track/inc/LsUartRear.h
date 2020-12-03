#pragma once

#include <DmaUartF4.h>
#include "LineGetterUart.h"

class LsUartRear : public LineGetterUart
{
public:
	static LsUartRear* GetInstance();

private:
	LsUartRear();
};
