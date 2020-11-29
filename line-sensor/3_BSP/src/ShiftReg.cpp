#include "ShiftReg.h"

#include "Spi.h"
#include "stm32f0xx_hal.h"


ShiftReg::ShiftReg(GpioPin OE_Pin, GpioPin LE_Pin)
{
	spi = Spi::GetInstance();

	this->LE_Pin  = 1 << Stm32Gpio::GetPin(LE_Pin);
	this->OE_Pin  = 1 << Stm32Gpio::GetPin(OE_Pin);
	this->LE_Port = Stm32Gpio::GetPort(LE_Pin);
	this->OE_Port = Stm32Gpio::GetPort(OE_Pin);

	GpioInit();

	HAL_GPIO_WritePin(this->OE_Port, this->OE_Pin, GPIO_PIN_RESET); // High active -> Set
	HAL_GPIO_WritePin(this->LE_Port, this->LE_Pin, GPIO_PIN_RESET); // Low active  -> Reset
}

bool ShiftReg::Display(void* data, size_t size)
{
	bool ret = false;

	if (spi->IsReady())
	{
		spi->Send(data, size);

		while(!spi->IsReady());

		HAL_GPIO_WritePin(LE_Port, LE_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LE_Port, LE_Pin, GPIO_PIN_RESET);

		ret = true;
	}

	return ret;
}

void ShiftReg::GpioInit()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// GPIO Ports Clock Enable (both)
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	// Configure OE pin
	GPIO_InitStruct.Pin = OE_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(OE_Port, &GPIO_InitStruct);

	// Configure LE pin
	GPIO_InitStruct.Pin = LE_Pin;

	HAL_GPIO_Init(LE_Port, &GPIO_InitStruct);
}
