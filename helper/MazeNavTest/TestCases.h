#pragma once
#include "MazeNavTest.h"

#define TEST_CASE_NUM (sizeof(testCases)/sizeof(testCases[0]))

NAV_TEST_CASE testCases[] =
{
    // Test case 0
    {
        .startSection = sApos,
        .stepNum = 3,
        .steps={
            {3, {adForward, edFrontLeft, false}},
            {3, {adForward, edFrontLeft, false}},
            {7, {adForward, edFrontLeft, false}}
        }
    },
    // Test case 1
    {
        .startSection = sApos,
        .stepNum = 3,
        .steps={
            {3, {adForward, edFrontLeft, false}},
            {3, {adForward, edFrontRight, false}},
            {7, {adForward, edFrontLeft, false}}
        }
    },
};
