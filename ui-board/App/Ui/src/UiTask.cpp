#include <MainUiMsg.h>
#include "UiTask.h"
#include "Matrix.h"
#include "Buttons.h"
#include "MainUart.h"
#include "FastTask.h"
#include <functional>
#include "BinTrace.h"

uint8_t UiTask::txBuf;

#define PRIO                 2
#define PERIOD              20
#define STACK              configMINIMAL_STACK_SIZE

UiTask::UiTask() : CyclicTask((char*)"Ui", PERIOD, PRIO, STACK),
		           cmdProc(MainUart::GetInstance(), cmdProcBuf, CMD_PROC_BUF_LEN, &enc)
{
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

	ProcessRxQueue();

	// Buttons
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

		U2M_SETTING_START msg;
		TRACE_BIN(&msg, sizeof(msg));
	}

	// Enter button
	if (buttons.GetRisingEdge(ButtonB) == true)
	{
		settingEnabled = false;

		matrix.SetBlinking(NO_BLINKING);

		ProcessRxQueue(); // Clear Rx buffer

		U2M_SET_SECTION msg;
		msg.section = (uint8_t)num;
		TRACE_BIN(&msg, sizeof(msg));
	}

	BinTrace::GetInstance().Process();
}

#define TYPE_SIZE_ASSERT(x) if (size != sizeof(x)) return

void UiTask::ProcessRxQueue()
{
	uint8_t buf[20];
	size_t size;

	while (true)
	{
		cmdProc.Receive(buf, size, 20);

		if (size > 0)
		{
			ProcessRxMessage(buf, size);
		}
		else
		{
			break;
		}
	}
}

void UiTask::ProcessRxMessage(uint8_t buf[], size_t size)
{
	// Capturing lambdas can't be assigned to function pointers.
	// https://stackoverflow.com/questions/28746744/passing-capturing-lambda-as-function-pointer/28746827
	static const std::function<void()> MsgProc[m2uCnt] =
	{
		[m2uReset]      = [&](){ TYPE_SIZE_ASSERT(M2U_RESET);
			                     NVIC_SystemReset();},

		[m2uSetSection] = [&](){ TYPE_SIZE_ASSERT(M2U_SET_SECTION);
			                     if (settingEnabled)
									 num = buf[1];}
	};

	if (size > 0)
	{
		uint8_t ID = buf[0];

		if (ID < m2uCnt)
		{
			MsgProc[ID]();
		}
	}
}
