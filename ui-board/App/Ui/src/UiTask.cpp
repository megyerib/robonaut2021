#include "UiTask.h"
#include "Matrix.h"
#include "Buttons.h"
#include "MainUart.h"
#include "FastTask.h"

uint8_t UiTask::txBuf;

#define PRIO                 2
#define PERIOD              20
#define STACK              configMINIMAL_STACK_SIZE

UiTask::UiTask() : CyclicTask((char*)"Ui", PERIOD, PRIO, STACK)
{
	Matrix::GetInstance();
	Buttons::GetInstance();
	MainUart::GetInstance();

	FastTask::Init();
}

UiTask& UiTask::Init()
{
	static UiTask instance;
	return instance;
}

void UiTask::TaskInit()
{

}

void UiTask::TaskFunction()
{
	Matrix&   matrix  = Matrix::GetInstance();
	Buttons&  buttons = Buttons::GetInstance();
	MainUart& uart    = MainUart::GetInstance();

	size_t size;
	uart.Receive(&rxBuf, size, 1); // Receive 1 byte
	if (size > 0 && !settingEnabled)
	{
		num = rxBuf;
		num = (num <= 99) ? num : 99;
	}

	if (settingEnabled)
	{
		// Set number
		if (buttons.GetRisingEdge(ButtonRight) == true) num++;
		if (buttons.GetRisingEdge(ButtonLeft)  == true) num--;
		if (buttons.GetRisingEdge(ButtonUp)    == true) num += 10;
		if (buttons.GetRisingEdge(ButtonDown)  == true) num -= 10;


		// Normalize number
		num = (num < 0) ? 0 : ((num > 99) ? 99 : num);
	}

	matrix.DisplayInt(num);

	// Back button
	if (buttons.GetRisingEdge(ButtonA) == true)
	{
		settingEnabled = true;

		matrix.SetBlinking(400);

		buttons.ClearRisingEdges();          // Clear previous button pushes

		txBuf = (uint8_t) 0xFF;              // Send pause sign
		uart.Transmit(&txBuf, 1);
	}

	// Enter button
	if (buttons.GetRisingEdge(ButtonB) == true)
	{
		settingEnabled = false;

		matrix.SetBlinking(NO_BLINKING);

		size_t size;                         // Clear Rx buffer
		uart.Receive(&rxBuf, size, 1);       // Receive 1 byte (dummy)

		txBuf = (uint8_t)num;                // Send number
		uart.Transmit(&txBuf, 1);
	}
}
