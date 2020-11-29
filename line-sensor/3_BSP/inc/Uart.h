#pragma once

#include "Stm32Uart.h"

#define BUFFER_MAX_SIZE  30

class Uart : public Stm32Uart
{
	uint8_t txBuffer[BUFFER_MAX_SIZE];
	size_t txBufSize = 0;

	Uart();

	void TxCompleteCallback() override;
	void RxCompleteCallback() override;
	void Init();

public:
	static Uart* GetInstance();

	void Send(uint8_t* buffer, size_t size);
};
