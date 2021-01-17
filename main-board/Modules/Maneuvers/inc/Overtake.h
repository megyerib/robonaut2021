#pragma once

#include <stdint.h>
#include "WaitDistance.h"
#include "TrackDetector.h"
#include "Inertial.h"

typedef enum : uint8_t
{
    ovrsIdle = 0U,
    ovrsPrepare,
    ovrsLeaveLine,
    ovrsGetBackParallel,
    ovrsPassSafetyCar,
    ovrsTurnBackToLine,
    ovrsSearchLine,
    ovrsSuccessfulOvertake,
    ovrsFailedOvertake
} OVERTAKE_STATE;

typedef enum : uint8_t
{
    ovrerrNoError = 0U,
    ovrerrOngoingOvertake,
    ovrerrSuccessfulOvertake,
    ovrerrFailedOvertake,

    ovrerrUnknown = 255U
} OVR_ERROR;

typedef struct
{
    float     speed;
    float     wheel_angle;
    OVR_ERROR error;
} OVERTAKE_INFO;

// IN
//  - distance elapsed
//  - line detector
//  - inertial z angular speed
//
// OUT
//  - speed
//  - wheel angle
//  - status

class Overtake
{
public:
    static Overtake& GetInstance();

    OVERTAKE_INFO Overtake_Process();

private:
    OVERTAKE_STATE  state;
    OVERTAKE_INFO   control_values;

    WaitDistance*   dist_waiter;
    TrackDetector*  line_detector;
    Inertial&       inert_sensor;
    // TODO PID controller

    Overtake();

    void StateMachine_OverTake();
    void State_Prepare();
    void State_LeaveLine();
    void State_GetParallelWithLine();
    void State_PassSafetyCar();
    void State_TurnBackToLine();
    void State_SearchLine();
    void State_SuccessfulOvertake();
    void State_FailedOvertake();
};

// Speed up (2m/s?)
// Turn wheel (20°?) for a distance (0.03m?)
// Turn wheel (-20°?) for a distance (0.03m?)
// Control wheel (manual 0° or PID on yaw orientation (z axis)
// Speed to >3m/s (3.5m/s?)
// Drive a distance (3-4m?)
// Slow down (2m/s?)
// Turn wheel (20°?) for a distance (0.03m?)
// Turn wheel (-20°?) for a distance (0.03m?)
// Line found = success
// No line = failed -> stop
