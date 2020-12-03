#include <string.h> /* memcpy */
#include <ShiftReg.h>

#define    MATRIX_LATCH_PIN         GPIO_PIN_0
#define    MATRIX_OE_PIN            GPIO_PIN_1
#define    MATRIX_RESET_PIN         GPIO_PIN_2
#define    LED_RESET_PIN            GPIO_PIN_3
#define    LED_LATCH_PIN            GPIO_PIN_4
#define    LED_OE_PIN               GPIO_PIN_6

ShiftReg::ShiftReg()
{
	InitSpiGpio();
	InitSpi();
	InitShiftRegGpio();

	// Reset pulse (Low active)
	HAL_GPIO_WritePin(GPIOA, MATRIX_RESET_PIN|LED_RESET_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, MATRIX_RESET_PIN|LED_RESET_PIN, GPIO_PIN_SET);

	// Enable outputs (Low active)
	HAL_GPIO_WritePin(GPIOA, MATRIX_OE_PIN|LED_OE_PIN, GPIO_PIN_RESET);
}

ShiftReg* ShiftReg::GetInstance()
{
	static ShiftReg instance;
	return &instance;
}

void ShiftReg::InitSpiGpio()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	/**SPI1 GPIO Configuration
	PA5     ------> SPI1_SCK
	PA7     ------> SPI1_MOSI
	*/
	GPIO_InitStruct.Pin       = GPIO_PIN_5|GPIO_PIN_7;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void ShiftReg::InitSpi()
{
	__HAL_RCC_SPI1_CLK_ENABLE();

	handle.Instance               = SPI1;
	handle.Init.Mode              = SPI_MODE_MASTER;
	handle.Init.Direction         = SPI_DIRECTION_2LINES;
	handle.Init.DataSize          = SPI_DATASIZE_8BIT;
	handle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
	handle.Init.CLKPhase          = SPI_PHASE_1EDGE;
	handle.Init.NSS               = SPI_NSS_SOFT;
	handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	handle.Init.TIMode            = SPI_TIMODE_DISABLE;
	handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	handle.Init.CRCPolynomial     = 7;
	handle.Init.CRCLength         = SPI_CRC_LENGTH_DATASIZE;
	handle.Init.NSSPMode          = SPI_NSS_PULSE_ENABLE;

	HAL_SPI_Init(&handle);

	/* SPI1 interrupt */
	HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(SPI1_IRQn);
}

void ShiftReg::InitShiftRegGpio()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();

	// PA0   MATRIX_LATCH   (High active)
	// PA1   MATRIX_OE      (Low active)
	// PA2   MATRIX_RESET   (Low active)
	// PA3   LED_RESET      (Low active)
	// PA4   LED_LATCH      (High active)
	// PA6   LED_OE         (Low active)

	GPIO_InitStruct.Pin       = MATRIX_LATCH_PIN|MATRIX_OE_PIN|MATRIX_RESET_PIN|LED_RESET_PIN|LED_LATCH_PIN|LED_OE_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOA, MATRIX_LATCH_PIN|LED_LATCH_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, MATRIX_OE_PIN|MATRIX_RESET_PIN|LED_RESET_PIN|LED_OE_PIN, GPIO_PIN_SET);
}

int32_t ShiftReg::Transmit(const void* buffer, size_t size)
{
	if (size <= LOCAL_BUFFER_SIZE && IsReady())
	{
		memcpy(localBuffer, buffer, size);
		HAL_SPI_Transmit_IT(&handle, localBuffer, size);

		return TRANSMIT_OK;
	}
	else
	{
		return TRANSMIT_BUSY;
	}
}

bool ShiftReg::IsReady()
{
	return (HAL_SPI_GetState(&handle) == HAL_SPI_STATE_READY);
}

void ShiftReg::HandleIrq()
{
	HAL_SPI_IRQHandler(&handle);
}

void ShiftReg::TxCompleteCallback()
{
	// Latch pulse (high active)
	HAL_GPIO_WritePin(GPIOA, MATRIX_LATCH_PIN|LED_LATCH_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, MATRIX_LATCH_PIN|LED_LATCH_PIN, GPIO_PIN_RESET);
}

extern "C" void SPI1_IRQHandler(void)
{
	ShiftReg::GetInstance()->HandleIrq();
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	ShiftReg::GetInstance()->TxCompleteCallback();
}
