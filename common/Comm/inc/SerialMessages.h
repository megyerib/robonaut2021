#pragma once
#include <stdint.h>

// SM == Serial Message
//
// This file shall be C compatible!

// Serial message types ------------------------------------------------------------------------------------------------

typedef uint8_t smID;

typedef enum
{
    smText = 0,
    smDummy,
	smTelemetry,
	smCfg,

    smNum
}
SM_ID;

// Message structs (fixed only) ----------------------------------------------------------------------------------------

#pragma pack(push,1)

// 0x00 Text
typedef struct
{
    smID id = smText;
    char text[];
}
SM_TEXT;

// 0x01 Dummy
typedef struct
{
    smID     id = smDummy;
    uint32_t timestamp; // [us]
    int32_t  value;
}
SM_DUMMY;

// 0x02 Telemetry
typedef struct
{
	smID     id = smTelemetry;
	uint8_t  type = 0x00; // Speed
	uint32_t timestamp;  // [us]
	uint32_t speed;      // [mm/s]
	uint32_t distance;   // [mm]
}
SM_TM_SPEED;

typedef struct
{
	smID     id = smTelemetry;
	uint8_t  type = 0x01; // Remote
	uint32_t timestamp;  // [us]
	int8_t  channel[3];
}
SM_TM_REMOTE;

#pragma pack(pop)

#define TRACE_DUMMY(x) SM_DUMMY msg; msg.timestamp = UPTIME_us(); msg.value = (x); TRACE_BIN(&msg, sizeof(msg))
