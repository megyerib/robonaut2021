#pragma once

#include <stdint.h>
#include "WaitDistance.h"
#include "TrackDetector.h"

typedef enum : uint8_t
{
    mesUninitialzed = 0U,
    mesInitialized,
    mesForwardExit_Prepare,
    mesForwardExit_LeaveLine,
    mesForwardExit_SearchLine,
    mesYExit_Prepare,
    mesYExit_LeaveLine,
    mesYExit_Reverse,
    mesYExit_SearchLine,
    mesSuccessfullyFinished,
    mesFailedToExit
} MAZE_EXIT_STATE;

typedef enum : uint8_t
{
    memForward = 0U,
    memY,

    memNotSet = 255U
} MAZE_EXIT_MODE;

typedef enum : uint8_t
{
    memerrNoError = 0U,
    memerrNotInitialized,
    memerrOngoingExitForward,
    memerrOngoingExitY,
    memerrSuccessfullyEnded,
    memerrFailedExit,
    memerrInvalidMode,

    memerrUnknown = 255U
} MEM_ERROR;

typedef struct
{
    float     speed;
    float     wheel_angle;
    MEM_ERROR error;
} EXIT_INFO;


class MazeExit
{
public:
static MazeExit& GetInstance();

MEM_ERROR Init(MAZE_EXIT_MODE const mode);

EXIT_INFO MazeExit_Process();

private:
    MAZE_EXIT_MODE  mode;
    MAZE_EXIT_STATE state;
    EXIT_INFO       control_values;
    bool            initialized;

    WaitDistance    dist_waiter;
    TrackDetector*  line_detector;

MazeExit();

void StateMachine_ExitForward();
void State_ExitForward_PrepareToLeaveLine();
void State_ExitForward_LeaveLine();
void State_ExitForward_SearchLine();

void StateMachine_ExitY();
void State_ExitY_PrepareToLeaveLine();
void State_ExitY_LeaveLine();
void State_ExitY_Reverse();
void State_ExitY_SearchLine();

void State_ExitSuccessful();
void State_ExitFailed();
};

