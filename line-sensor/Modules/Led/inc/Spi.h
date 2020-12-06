#pragma once

#include "stm32f0xx_hal.h"

#define LOCAL_BUFFER_SIZE (8u)  /* Rev1 sensor needs double size */

class Spi
{
public:
	static Spi* GetInstance();
	void Send(void* data, size_t size);
	bool IsReady();

private:
	static SPI_HandleTypeDef handle;
	uint8_t localBuffer[LOCAL_BUFFER_SIZE];

	Spi();

	void InitGpio();
	void InitSpi();
};
