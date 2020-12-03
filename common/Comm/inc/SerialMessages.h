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

    smNum
}
SM_ID;

// Message structs (fixed only) ----------------------------------------------------------------------------------------

#pragma pack(push,1)

// 0x00 Text
typedef struct
{
    smID id;
    char text[];
}
SM_TEXT;

// 0x01 Dummy
typedef struct
{
    smID id;
    uint32_t timestamp; // [us]
    int32_t  value;
}
SM_DUMMY;

#pragma pack(pop)
