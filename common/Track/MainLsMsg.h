#pragma once

#include <stdint.h>
#include "Line.h"

typedef enum
{
	m2lReset = 0,

	m2uCnt
}
MAIN_TO_UI_MSG;

typedef enum
{
	l2mLineData = 0,

	u2mCnt
}
UI_TO_MAIN_MSG;

#pragma pack(push,1)

// Main to UI ------------------------------------------------------------------

typedef struct
{
	uint8_t ID;
}
M2L_RESET;

// UI to Main ------------------------------------------------------------------

typedef struct
{
	uint8_t   ID = l2mLineData;
	LineInput line;
}
L2M_LINE_DATA;

// -----------------------------------------------------------------------------

#pragma pack(pop)
