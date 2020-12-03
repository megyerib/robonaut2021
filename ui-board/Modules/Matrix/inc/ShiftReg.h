#pragma once

#include "stm32f0xx_hal.h"
#include "Transmitter.h"

#define LOCAL_BUFFER_SIZE (8u)  /* Rev1 sensor needs double size */

class ShiftReg : public Transmitter
{
public:
	static ShiftReg* GetInstance();
	virtual int32_t Transmit(const void* buffer, size_t size) override;
	bool IsReady();

	void HandleIrq();
	void TxCompleteCallback();

private:
	SPI_HandleTypeDef handle;
	uint8_t localBuffer[LOCAL_BUFFER_SIZE];

	ShiftReg();

	void InitSpiGpio();
	void InitSpi();
	void InitShiftRegGpio();
};
