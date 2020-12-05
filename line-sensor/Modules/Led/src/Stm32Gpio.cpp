#include "Stm32Gpio.h"

GPIO_TypeDef* Stm32Gpio::GetPort(GpioPin pin)
{
	uint32_t index = pin >> 4;

	if (index < GPIO_PORT_NUM)
	{
		return GpioPorts[index];
	}
	else
	{
		return nullptr;
	}
}

uint16_t Stm32Gpio::GetPin(GpioPin pin)
{
	return pin & 0xF;
}
