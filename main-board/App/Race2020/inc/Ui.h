#pragma once

#include "UiUart.h"

// CAUTION! This code is not very nice.
// Make sure you call the GetCommand/IsStopped
// command between every button push!

class Ui
{
public:
	static Ui* GetInstance();

	bool IsStopped();
	bool GetCommand(uint8_t* cmd);
	void SetCommand(uint8_t command);

private:
	uint8_t lastChar  = 0;
	bool stopped      = false;
	bool stateNew     = false;

	UiUart* uart;
	Ui();

	void Receive();
};
