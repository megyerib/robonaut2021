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

StarterData Starter::GetState()
{
	uint8_t c;
	size_t receivedChars;
	StarterData ret;

	ret.state = stNothing;

	uart->Receive(&c, receivedChars, 1);

	if (receivedChars > 0)
	{
		if (c >= '0' && c <= '5') // Countdown
		{
			ret.state     = stCountdown;
			ret.countdown = c - '0';
		}
		else if (c >= 'A' && c <= 'O') // Gate
		{
			ret.state = stGate;
			ret.gate  = c - 'A' + 1; // 'A' == Gate 1 (starter is Gate 0)
		}
		else
		{
			ret.state = stError; // Invalid character
		}
	}

	return ret;
}
