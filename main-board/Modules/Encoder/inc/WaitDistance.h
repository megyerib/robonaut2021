#pragma once

#include "Encoder.h"

class WaitDistance
{
public:
	WaitDistance();
	void Wait_m(float m);
	bool IsExpired();
	float GetDiff();
private:
	float startDistance = 0;
	float endDistance   = 0;
	bool started        = false;
	Encoder* encoder;

	bool positive       = true;
};
