#include <LedDriver.h>
#include "SensorCfg.h"
#include <stdint.h>

LedDriver::LedDriver() : leds(LED_OE_port, LED_OE_pin, LED_LE_port, LED_LE_pin)
{
	Clear();
}

LedDriver& LedDriver::GetInstance()
{
	static LedDriver instance;
	return instance;
}

void LedDriver::DisplayLinePos(LineInput l)
{
	uint32_t ledval = 0, i, ledpos = 0;

	for (i = 0; i < l.cnt; i++)
	{
		ledpos = mmToLedPos(l.lines[i]);

		ledval |= 0b11 << ledpos;
	}

	if (l.cross)
	{
		ledval = 0x55555555;
	}

	DisplayPattern(ledval);
}

void LedDriver::DisplayPattern(uint32_t pattern)
{
#if (SENSOR_REV == 1)
	uint32_t spi_buf[2];
	spi_buf[0] = transformReg_Rev1(pattern);
	leds.LedDriver(spi_buf, GROUP_CNT * 2);
#else
	leds.Send(&pattern, GROUP_CNT);
#endif
}

uint8_t LedDriver::mmToLedPos(int16_t mm)
{
    return (MID_IR_POS_MM - mm) / IR_DIST_MM;
}

void LedDriver::Clear()
{
	uint32_t buf = 0;

	leds.Send((uint8_t*)&buf, GROUP_CNT);
}

uint32_t LedDriver::transformReg_Rev1(uint32_t ledval)
{
	// Swap the two 16 bit halves
    uint16_t  tmp;
    uint16_t* words = (uint16_t*) &ledval;

    tmp = words[0];
    words[0] = words[1];
    words[1] = tmp;

    return ledval;
}
