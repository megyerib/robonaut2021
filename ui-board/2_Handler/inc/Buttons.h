#pragma once

#include <cstdint>
#include "stm32f0xx_hal.h"

#define DEBOUNCING_FILTER_SIZE  8

typedef enum
{
	ButtonDown  = GPIO_PIN_3,
	ButtonB     = GPIO_PIN_4,
	ButtonRight = GPIO_PIN_6,
	ButtonA     = GPIO_PIN_7,
	ButtonUp    = GPIO_PIN_8,
	ButtonLeft  = GPIO_PIN_9,

	ButtonNum
}
Button;

class Buttons
{
public:
	static Buttons* GetInstance();

	bool GetValue(Button b);
	bool GetRisingEdge(Button b);
	void ClearRisingEdges();
	void Process();

private:
	uint32_t risingEdge = 0;

	uint32_t values[DEBOUNCING_FILTER_SIZE] = {0};
	uint32_t index = 0;

	uint32_t filtered = 0;

	Buttons();

	void InitGpio();
};
