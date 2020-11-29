#include <TestLoop.h>
#include "Display.h"
#include "SensorDriver.h"

#include "Line.h"

void TestLoop::Run()
{
	SensorDriver sensors;
	Display      display;

	while(1)
	{
		static uint32_t buf = 1;
		static uint32_t shifter = 1;

		sensors.DisplayPattern(0);
		display.DisplayPattern(buf);

		// BREAKPOINT

		if (buf == 0xFFFFFFFF)
		{
			shifter = 0;
		}
		if (buf == 0)
		{
			shifter = 1;
		}

		buf <<= 1;
		buf  |= shifter;

		HAL_Delay(30);
	}
}

void TestLoop::RunLineTest()
{
	SensorDriver sensors;
	Display      display;

	LineInput l;
	l.cnt = 1;
	l.lines[0] = -150;

	sensors.DisplayPattern(0);

	while(1)
	{
		display.DisplayLinePos(l);

		l.lines[0] += 1;

		if (l.lines[0] > 150)
		{
			l.lines[0] = -150;
		}

		HAL_Delay(3);
	}
}
