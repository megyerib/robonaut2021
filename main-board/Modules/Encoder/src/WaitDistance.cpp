#include "WaitDistance.h"

WaitDistance::WaitDistance()
{
	encoder = Encoder::GetInstance();
}

void WaitDistance::Wait_m(float m)
{
	startDistance = encoder->GetDistance();
	endDistance = startDistance + m;

	positive = (m > 0);

	started = true;
}

bool WaitDistance::IsExpired()
{
	bool ret;

	if (positive)
	{
		ret = started && (endDistance <= encoder->GetDistance());
	}
	else
	{
		ret = started && (endDistance >= encoder->GetDistance());
	}

	return ret;
}

float WaitDistance::GetDiff()
{
	return encoder->GetDistance() - startDistance;
}
