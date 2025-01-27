#include "ShiftReg.h"

#include "Spi.h"
#include "stm32f0xx_hal.h"


ShiftReg::ShiftReg(GPIO_TypeDef* OE_Port,
				   uint32_t      OE_Pin,
				   GPIO_TypeDef* LE_Port,
                   uint32_t      LE_Pin)
{
	spi = Spi::GetInstance();

	this->LE_Pin  = LE_Pin;
	this->OE_Pin  = OE_Pin;
	this->LE_Port = LE_Port;
	this->OE_Port = OE_Port;

	GpioInit();

	HAL_GPIO_WritePin(this->OE_Port, this->OE_Pin, GPIO_PIN_RESET); // Low active -> Set
	HAL_GPIO_WritePin(this->LE_Port, this->LE_Pin, GPIO_PIN_RESET); // High active  -> Reset
}

bool ShiftReg::Send(void* data, size_t size)
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

void ShiftReg::Enable(bool en)
{
	HAL_GPIO_WritePin(this->OE_Port, this->OE_Pin, en ? GPIO_PIN_RESET : GPIO_PIN_SET); // Low active
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
