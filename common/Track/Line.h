#pragma once

#include <stdint.h>

#define MAXLINES      (3u)

#define UNIT_TO_M  (4.27e-6f)
#define M_TO_UNIT  (234192) /* 1 / UNIT_TO_M */

typedef struct
{
    int16_t lines[MAXLINES];
    uint8_t cnt;
    uint8_t cross;
}
LineInput;
