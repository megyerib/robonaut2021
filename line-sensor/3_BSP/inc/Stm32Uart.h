#pragma once

#ifdef STM32F446xx // For portability
#include "stm32f4xx_hal.h"

typedef enum // Unavailable UARTs can be deleted
{
	Uart1 = 0,
	Uart2 = 1,
	Uart3 = 2,
	Uart4 = 3,
	Uart5 = 4,
	Uart6 = 5,

	UartMaxNum
}
UartInstance;
#endif

#ifdef STM32F030xx // For portability
#include "stm32f0xx_hal.h"

typedef enum // Unavailable UARTs can be deleted
{
	Uart1 = 0,

	UartMaxNum
}
UartInstance;
#endif

class Stm32Uart
{
public:
	static void IrqHandler(UartInstance inst);

	static void CallTxCompleteCallback(UART_HandleTypeDef* huart);
	static void CallRxCompleteCallback(UART_HandleTypeDef* huart);

protected:
	UART_HandleTypeDef handle;

	Stm32Uart(UartInstance inst);

private:
	static Stm32Uart* instances[UartMaxNum];
	static uint32_t instanceNum;

	static bool HandleToInstance(UART_HandleTypeDef* huart, UartInstance* result);

	virtual void TxCompleteCallback() = 0;
	virtual void RxCompleteCallback() = 0;
};
