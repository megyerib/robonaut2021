#include "System.h"
#include "MatrixDisplay.h"
#include "stm32f0xx_hal.h"
#include "Buttons.h"
#include "MainUart.h"

#define BLINK_CNTR_PERIOD 150

int main(void)
{
	System::Init();

	MatrixDisplay* matrix = MatrixDisplay::GetInstance();
	Buttons* buttons = Buttons::GetInstance();
	MainUart& uart = MainUart::GetInstance();

	int num = 0;
	bool blinking = false;
	int cntr = 0;
	bool showNum = true;
	static uint8_t txBuf;
	uint8_t rxBuf;

	while (1)
	{
		bool buttonPushed = false;

		size_t size;
		uart.Receive(&rxBuf, size, 1); // Receive 1 byte
		if (size > 0 && !blinking)
		{
			num = rxBuf;
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
			uart.Receive(&rxBuf, size, 1); // Receive 1 byte

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

		HAL_Delay(1);
	}

	return 0;
}
