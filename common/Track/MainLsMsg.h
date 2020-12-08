#pragma once

#include <stdint.h>
#include "Line.h"

typedef enum
{
	m2lReset = 0,
	m2lCfg,

	m2lCnt
}
MAIN_TO_UI_MSG;

typedef enum
{
	l2mLineData = 0,
	l2mSensorData,

	l2mCnt
}
UI_TO_MAIN_MSG;

#pragma pack(push,1)

// Main to UI ------------------------------------------------------------------

typedef struct
{
	uint8_t ID = m2lReset;
}
M2L_RESET;

typedef struct
{
	uint8_t ID = m2lCfg;
	uint8_t LineDataEn   : 1;
	uint8_t SensorDataEn : 1;
	uint8_t LedEn        : 1;
	uint8_t MeasEn       : 1;
	uint8_t              : 4; // Padding
}
M2L_CFG;

// UI to Main ------------------------------------------------------------------

typedef struct
{
	uint8_t   ID = l2mLineData;
	LineInput line;
}
L2M_LINE_DATA;

typedef struct
{
	uint8_t  ID = l2mSensorData;
	uint8_t  size;
	uint16_t data[];
}
L2M_SENSOR_DATA;

// -----------------------------------------------------------------------------

#pragma pack(pop)
