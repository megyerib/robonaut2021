#pragma once

#include "MazeNavBase.h"

#define MAX_STEPS 20

typedef struct
{
    MAZE_SECTION startSection;
    size_t stepNum;
    struct {
        uint8_t target;
        MAZE_MOVE expRes; // Expected result
    } steps[MAX_STEPS];
}
NAV_TEST_CASE;

class MazeNavTest
{
public:
    static bool Test(MazeNavBase& nav, NAV_TEST_CASE& testCase);
};
