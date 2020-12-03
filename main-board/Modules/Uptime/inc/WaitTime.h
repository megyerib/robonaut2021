#pragma once
#include <cstdint>

class WaitTime
{
public:
	WaitTime();
	void Wait_s(int s);
	void Wait_ms(int ms);
	void Wait_us(int us);
	bool IsExpired();

private:
	uint32_t endTime;
	bool started = false;
};
