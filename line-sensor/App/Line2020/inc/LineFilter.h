#pragma once
#include "Line.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
Example:
recentIndex = 3

meas[0]    t-3
meas[1]    t-2
meas[2]    t-1
meas[3] <- t   (most recent)
meas[4]    t-4 (least recent)
*/

LineInput FilterLines(LineInput meas[5], size_t recentIndex);

#ifdef __cplusplus
}
#endif
