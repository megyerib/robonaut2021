#include "WaitTime.h"
#include "Uptime.h"

WaitTime::WaitTime()
{

}

void WaitTime::Wait_s(int s)
{
	Wait_us(s * 1000000);
}

void WaitTime::Wait_ms(int ms)
{
	Wait_us(ms * 1000);
}

void WaitTime::Wait_us(int us)
{
	endTime = UPTIME_us() + us;
	started = true;
}

bool WaitTime::IsExpired()
{
	return (started && (endTime <= UPTIME_us()));
}
