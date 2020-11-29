#include "Display.h"
#include "SensorCfg.h"
#include <stdint.h>

Display::Display() : leds(LED_OE, LED_LE)
{
	Clear();
}

void Display::DisplayLinePos(LineInput l)
{
	uint32_t ledval = 0, i, ledpos = 0;

	for (i = 0; i < l.cnt; i++)
	{
		ledpos = mmToLedPos(l.lines[i]);

		ledval |= 0b11 << ledpos;
	}

	if (l.cross)
	{
		ledval = 0xFFFFFFFF;
	}

	DisplayPattern(ledval);
}

void Display::DisplayPattern(uint32_t pattern)
{
#if (SENSOR_REV == 1)
	uint32_t spi_buf[2];
	spi_buf[0] = transformReg_Rev1(pattern);
	leds.Display(spi_buf, GROUP_CNT * 2);
#else
	leds.Display(&pattern, GROUP_CNT);
#endif
}

uint8_t Display::mmToLedPos(int16_t mm)
{
    return (MID_IR_POS_MM - mm) / IR_DIST_MM;
}

void Display::Clear()
{
	uint32_t buf = 0;

	leds.Display((uint8_t*)&buf, GROUP_CNT);
}

uint32_t Display::transformReg_Rev1(uint32_t ledval)
{
	// Swap the two 16 bit halves
    uint16_t  tmp;
    uint16_t* words = (uint16_t*) &ledval;

    tmp = words[0];
    words[0] = words[1];
    words[1] = tmp;

    return ledval;
}
