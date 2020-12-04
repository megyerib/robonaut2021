#pragma once

#include <stdint.h>

typedef enum
{
	m2uReset = 0,
	m2uSetSection,

	m2uCnt
}
MAIN_TO_UI_MSG;

typedef enum
{
	u2mSettingStart = 0,
	u2mSetSection,

	u2mCnt
}
UI_TO_MAIN_MSG;

#pragma pack(push,1)

// Main to UI ------------------------------------------------------------------

typedef struct
{
	uint8_t ID;
}
M2U_RESET;

typedef struct
{
	uint8_t ID;
	uint8_t section;
}
M2U_SET_SECTION;

// UI to Main ------------------------------------------------------------------

typedef struct
{
	uint8_t ID = u2mSettingStart;
}
U2M_SETTING_START;

typedef struct
{
	uint8_t ID = u2mSetSection;
	uint8_t section;
}
U2M_SET_SECTION;

// -----------------------------------------------------------------------------

#pragma pack(pop)
