#include "Buttons.h"
#include "stm32f0xx_hal.h"

Buttons* Buttons::GetInstance()
{
	static Buttons instance;
	return &instance;
}

bool Buttons::GetValue(Button b)
{
	uint32_t mask = (uint32_t)b;
	bool ret = ((filtered & mask) != 0);

	return ret;
}

bool Buttons::GetRisingEdge(Button b)
{
	uint32_t mask = (uint32_t)b;
	bool ret = ((risingEdge & b) != 0);

	risingEdge &= (~mask); // Clear edge

	return ret;
}

void Buttons::Process()
{
	uint32_t prevFiltered = filtered;

	// Set index
	index++;
	index %= DEBOUNCING_FILTER_SIZE;

	// Read
	values[index] = ~(GPIOB->IDR); // Negate bc of low active logic

	// Filter
	uint32_t zeros = 0x00000000;
	uint32_t ones  = 0xFFFFFFFF;

	for (int i = 0; i < DEBOUNCING_FILTER_SIZE; i++)
	{
		zeros |= values[i];
		ones  &= values[i];
	}

	filtered &= zeros;
	filtered |= ones;

	// Detect rising edge
	uint32_t newEdge = (~prevFiltered) & filtered;
	risingEdge |= newEdge;
}

Buttons::Buttons()
{
	InitGpio();
}

void Buttons::InitGpio()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Pin   = ButtonDown|ButtonB|ButtonRight|
			                ButtonA|ButtonUp|ButtonLeft;
	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Buttons::ClearRisingEdges()
{
    risingEdge = 0;
}
