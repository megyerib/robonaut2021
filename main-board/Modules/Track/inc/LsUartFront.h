#pragma once

#include "LineGetterUart.h"

class LsUartFront : public LineGetterUart
{
public:
	static LsUartFront* GetInstance();

private:
	LsUartFront();
};
