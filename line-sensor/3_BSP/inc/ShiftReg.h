#pragma once

#include <stddef.h>

#include "Spi.h"
#include "Stm32Gpio.h"

class ShiftReg
{
	Spi* spi;

	GPIO_TypeDef* LE_Port;
	GPIO_TypeDef* OE_Port;
	uint32_t LE_Pin;
	uint32_t OE_Pin;

	void GpioInit();

public:
	ShiftReg(GpioPin OE_Pin, GpioPin LE_Pin);
	bool Display(void* data, size_t size);
};
