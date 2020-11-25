#include "Starter.h"
#include "StarterUart.h"

Starter::Starter()
{
	uart = StarterUart::GetInstance();
}

Starter* Starter::GetInstance()
{
	static Starter instance;
	return &instance;
}

StarterState Starter::GetState()
{
	uint8_t c = '\0';
	size_t receivedChars;

	uart->Receive(&c, receivedChars, 1);

	if (receivedChars > 0)
	{
		switch (c)
		{
			case '5': state = Countdown5; break;
			case '4': state = Countdown4; break;
			case '3': state = Countdown3; break;
			case '2': state = Countdown2; break;
			case '1': state = Countdown1; break;
			case '0': state = Go;         break;
			default:  state = Error;      break;
		}
	}

	return state;
}
