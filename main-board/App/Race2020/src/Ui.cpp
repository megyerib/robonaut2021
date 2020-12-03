#include "Ui.h"

Ui::Ui()
{
	uart = UiUart::GetInstance();
}

Ui* Ui::GetInstance()
{
	static Ui instance;
	return &instance;
}

bool Ui::IsStopped()
{
	Receive();
	return stopped;
}

bool Ui::GetCommand(uint8_t* cmd)
{
	Receive();
	*cmd = lastChar;

	bool ret     = stateNew;
	stateNew     = false;
	return ret;
}

void Ui::SetCommand(uint8_t command)
{
	uart->Transmit(&command, 1);
}

void Ui::Receive()
{
	size_t size;
	uint8_t buf;
	uart->Receive(&buf, size, 1);

	if (size > 0)
	{
		if (buf == 255)
		{
			stopped = true;
		}
		else
		{
			lastChar     = buf;
			stopped      = false;
			stateNew     = true;
		}
	}
}
