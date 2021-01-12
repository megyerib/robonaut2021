#pragma once
#include "MazeNavTest.h"

#define TEST_CASE_NUM (sizeof(testCases)/sizeof(testCases[0]))

NAV_TEST_CASE testCases[] =
{
//    #define BACKWARD_PENALTY         (1.10F)
//    #define DIRECTION_CHANGE_PENALTY (1.05F)
    // Test case 0
    {
        .startSection = sStpos,
        .stepNum = 2,
        .steps={
            {13, {adBackward, edRearRight, false}},
            {13, {adBackward, edFrontLeft, false}}
        }
    },
    // Test case 1
    {
        .startSection = sNpos,
        .stepNum = 2,
        .steps={
            {8, {adBackward, edRearMid, false}},
            {8, {adBackward, edRearLeft, false}}
        }
    },
    // Test case 2
    {
        .startSection = sIpos,
        .stepNum = 7,
        .steps={
            {17, {adBackward, edRearMid,   false}},
            {17, {adBackward, edRearRight, false}},
            {17, {adBackward, edRearMid,   false}},
            {17, {adBackward, edRearLeft,  false}},
            {0, {adBackward, edRearRight,  false}},
            {0, {adBackward, edRearMid,    false}},
            {0, {adBackward, edRearRight,  false}}
        }
    },
    // Test case 3
    {
        .startSection = sAneg,
        .stepNum = 9,
        .steps={
            {23, {adForward, edFrontRight,false}},
            {23, {adForward, edFrontLeft, false}},
            {23, {adForward, edFrontMid,  false}},
            {23, {adForward, edFrontMid,  false}},
            {23, {adForward, edFrontMid,  false}},
            {23, {adForward, edFrontMid,  false}},
            {23, {adForward, edFrontLeft, false}},
            {23, {adForward, edFrontLeft, false}},
            {23, {adForward, edFrontMid,  false}}
        }
    },

    // Test case 4
    {
        .startSection = sDpos,
        .stepNum = 5,
        .steps={
            {19, {adForward,  edFrontMid,   false}},
            {19, {adForward,  edFrontLeft,  false}},
            {19, {adForward,  edFrontMid,   false}},
            { 7, {adBackward, edRearRight,  false}},
            { 7, {adBackward, edFrontRight, false}}
        }
    },

    // Test case 5
    {
        .startSection = sHpos,
        .stepNum = 7,
        .steps={
            { 8, {adForward,  edFrontMid,  false}},
            { 8, {adForward,  edRearLeft,  false}},
            {16, {adBackward, edRearMid,   false}},
            {16, {adBackward, edRearRight, false}},
            {16, {adBackward, edRearMid,   false}},
            {16, {adBackward, edRearLeft,  false}},
            {16, {adBackward, edRearLeft,  false}}
        }
    },
};
