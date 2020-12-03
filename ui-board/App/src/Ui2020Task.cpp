#include "Ui2020Task.h"
#include "Matrix.h"
#include "Buttons.h"
#include "MainUart.h"

uint8_t Ui2020Task::txBuf;

#define BLINK_CNTR_PERIOD  200

Ui2020Task::Ui2020Task() : CyclicTask((char*)"Ui2020", PERIOD, PRIO, STACK)
{

}

Ui2020Task& Ui2020Task::Init()
{
	static Ui2020Task instance;
	return instance;
}

void Ui2020Task::TaskInit()
{

}

void Ui2020Task::TaskFunction()
{
	Matrix*   matrix  = Matrix::GetInstance();
	Buttons*  buttons = Buttons::GetInstance();
	MainUart& uart    = MainUart::GetInstance();

	bool buttonPushed = false;

	size_t size;
	uart.Receive(&rxBuf, size, 1); // Receive 1 byte
	if (size > 0 && !blinking)
	{
		num = rxBuf;
		num = (num <= 99) ? num : 99;
	}

	if (blinking)
	{
		// Set number
		if (buttons->GetRisingEdge(ButtonRight) == true)
		{
			num++;
			buttonPushed = true;
		}
		if (buttons->GetRisingEdge(ButtonLeft) == true)
		{
			num--;
			buttonPushed = true;
		}
		if (buttons->GetRisingEdge(ButtonUp) == true)
		{
			num += 10;
			buttonPushed = true;
		}
		if (buttons->GetRisingEdge(ButtonDown) == true)
		{
			num -= 10;
			buttonPushed = true;
		}

		if (buttonPushed)
		{
			if (num < 0)
			{
				num = 0;
			}
			else if (num > 99)
			{
				num = 99;
			}
		}
	}

	// Back button
	if (buttons->GetRisingEdge(ButtonA) == true)
	{
		blinking = true;
		cntr = 0;
		showNum = false;
		buttons->ClearRisingEdges();

		txBuf = (uint8_t)255;
		uart.Transmit(&txBuf, 1);
	}

	// Enter button
	if (buttons->GetRisingEdge(ButtonB) == true)
	{
		blinking = false;
		showNum = true;

		// Clear Rx buffer
		size_t size;
		uart.Receive(&rxBuf, size, 1); // Receive 1 byte (dummy)

		txBuf = (uint8_t)num;
		uart.Transmit(&txBuf, 1);
	}

	// Count flashing
	if (blinking)
	{
		cntr++;

		if (cntr >= BLINK_CNTR_PERIOD)
		{
			cntr = 0;
			showNum = !showNum;
		}
	}

	// Display
	if (showNum)
	{
		matrix->DisplayInt(num);
	}
	else
	{
		matrix->Clear();
	}

	matrix->Refresh();

	buttons->Process();
}
