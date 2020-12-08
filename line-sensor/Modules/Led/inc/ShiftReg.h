#pragma once

#include <stddef.h>
#include "Spi.h"

class ShiftReg
{
public:
	ShiftReg(GPIO_TypeDef* OE_Port,
			 uint32_t      OE_Pin,
			 GPIO_TypeDef* LE_Port,
	         uint32_t      LE_Pin);

	bool Send(void* data, size_t size);
	void Enable(bool en);

private:
	Spi* spi;

	GPIO_TypeDef* LE_Port;
	GPIO_TypeDef* OE_Port;
	uint32_t      LE_Pin;
	uint32_t      OE_Pin;

	void GpioInit();


};
