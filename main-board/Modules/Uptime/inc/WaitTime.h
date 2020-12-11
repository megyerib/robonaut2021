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
	void Stop();

private:
	uint32_t endTime = 0;
	bool started = false;
	bool endRead = false;
};
