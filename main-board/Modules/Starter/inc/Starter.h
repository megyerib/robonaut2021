#pragma once

#include "Receiver.h"

typedef enum
{
	stNothing = 0,
	stCountdown,
	stGate,
	stError
}
StarterState;

typedef struct
{
	StarterState state;
	uint8_t      countdown;
	uint8_t      gate;
}
StarterData;

class Starter
{
public:
	static Starter* GetInstance();
	StarterData GetState();

private:
	Receiver* uart;

	Starter();
};
