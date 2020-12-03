#pragma once

class System
{
	System();
	void Clock_Config();

public:
	static void GetInstance();
};

// + SysTick handler in the source
