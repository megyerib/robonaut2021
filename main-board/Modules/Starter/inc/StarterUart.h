#pragma once

#include "stm32f4xx_hal.h"
#include "Receiver.h"

class StarterUart : public Receiver
{
public:
	static StarterUart* GetInstance();

	virtual int32_t Receive(void* buffer, size_t& size, size_t targetSize) override;

private:
	UART_HandleTypeDef huart;

	StarterUart();

	void InitUart();
	static void InitGpio();
};
