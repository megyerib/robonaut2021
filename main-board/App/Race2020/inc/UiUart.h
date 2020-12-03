#pragma once

#include "stm32f4xx_hal.h"
#include "Receiver.h"
#include "Transmitter.h"

class UiUart : public Receiver, public Transmitter
{
public:
	static UiUart* GetInstance();

	virtual int32_t Receive(void* buffer, size_t& size, size_t targetSize) override;
	virtual int32_t Transmit(const void* buffer, size_t size) override;

private:
	UART_HandleTypeDef huart;

	UiUart();

	void InitUart();
	static void InitGpio();
};
