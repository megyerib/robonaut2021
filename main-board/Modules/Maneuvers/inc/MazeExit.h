#pragma once

#include <stdint.h>
#include "WaitDistance.h"
#include "TrackDetector.h"

typedef enum : uint8_t
{
    mesUninitialzed = 0U,
    mesInitialized,
    mesQ2LeftExit_Prepare,
    mesQ2LeftExit_LeaveLine,
    mesQ2LeftExit_SearchLine,
    mesQ2RightExit_Prepare,
    mesQ2RightExit_LeaveLine,
    mesQ2RightExit_Reverse,
    mesQ2RightExit_SearchLine,
    mesSuccessfullyFinished,
    mesFailedToExit
} MAZE_EXIT_STATE;

typedef enum : uint8_t
{
    memQ2Left = 0U,
    memQ2Right,
    memLeft,
    memRight,

    memNotSet = 255U
} MAZE_EXIT_MODE;

typedef enum : uint8_t
{
    memerrNoError = 0U,
    memerrNotInitialized,
    memerrOngoingExitLeftQ2,
    memerrOngoingExitRightQ2,
    memerrOngoingExitLeft,
    memerrOngoingExitRight,
    memerrSuccessfullyEnded,
    memerrFailedExit,

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

EXIT_INFO MazeExit_Process_Q2();
EXIT_INFO Exit_Process();

private:
    MAZE_EXIT_MODE  mode;
    MEM_ERROR       error_code;
    MAZE_EXIT_STATE state;
    EXIT_INFO       control_values;
    bool            initialized;

    WaitDistance    dist_waiter;
    TrackDetector*  line_detector;

MazeExit();

void StateMachine_Q2ExitLeft();
void StateQ2_ExitLeft_PrepareToLeaveLine();
void StateQ2_ExitLeft_LeaveLine();
void StateQ2_ExitLeft_SearchLine();

void StateMachine_Q2ExitRight();
void StateQ2_ExitRight_PrepareToLeaveLine();
void StateQ2_ExitRight_LeaveLine();
void StateQ2_ExitRight_Reverse();
void StateQ2_ExitRight_SearchLine();

void StateMachine_ExitLeft();

void StateMachine_ExitRigth();

void State_ExitSuccessful();
void State_ExitFailed();
};

