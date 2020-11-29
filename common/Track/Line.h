#pragma once

#include <stdint.h>

#define MAXLINES      (3u)

typedef struct
{
    int16_t lines[MAXLINES];
    uint8_t cnt;
    uint8_t cross;
}
LineInput;
