#pragma once

#include "stm32f4xx_hal.h"

#define DATA_BUF_SIZE  (50u) /* Larger buffer -> less DMA IRQ */

typedef enum
{
	RemCh1 = 0,
	RemCh2,
	RemCh3,

	RemChNum
}
RemoteHwChannel;

class RemoteHw
{
public:
	static RemoteHw* GetInstance();
	uint16_t GetPulseWidth(RemoteHwChannel ch); /* us */

	void Ch1DmaIrq();
	void Ch2DmaIrq();
	void Ch4DmaIrq();

private:
	static uint16_t data[RemChNum][DATA_BUF_SIZE];
	uint16_t prev[RemChNum] = {0};

	TIM_HandleTypeDef htim;

	DMA_HandleTypeDef hdma[RemChNum];

	RemoteHw();

	void InitGpio();
	void InitTimer();
	void InitDma();
};
