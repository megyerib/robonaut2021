#pragma once

#include "Receiver.h"

typedef enum
{
	Go,
	Countdown1,
	Countdown2,
	Countdown3,
	Countdown4,
	Countdown5,
	NoSignal,
	Error
}
StarterState;

class Starter
{
public:
	static Starter* GetInstance();
	StarterState GetState();

private:
	Receiver* uart;
	StarterState state = NoSignal;

	Starter();
};
