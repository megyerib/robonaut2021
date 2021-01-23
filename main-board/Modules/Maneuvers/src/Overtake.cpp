#include "Overtake.h"
#include "Overtake_Settings.h"
#include "Trace.h"

Overtake& Overtake::GetInstance()
{
    static Overtake instance;
    return instance;
}

OVERTAKE_INFO Overtake::Overtake_Process()
{
    StateMachine_OverTake();

    return control_values;
}

Overtake::Overtake() : state(ovrsIdle),
                       control_values({0.0f, 0.0f, ovrerrNoError}),
                       dist_waiter(new WaitDistance()),
                       line_detector(TrackDetector::GetInstance()),
                       inert_sensor(Inertial::GetInstance())
{}

void Overtake::StateMachine_OverTake()
{
    switch (state)
    {
        case ovrsIdle:
        {
            control_values.error = ovrerrNoError;

            dist_waiter->Wait_m(OVERTAKE_PREPARE_WAIT_DIST);

#ifdef OVERTAKE_TRACE_ENABLED
        PRINTF("Overtake: Preparing.\n");
#endif

            state = ovrsPrepare;
            break;
        }
        case ovrsPrepare:
        {
            State_Prepare();
            break;
        }
        case ovrsLeaveLine:
        {
            State_LeaveLine();
            break;
        }
        case ovrsGetBackParallel:
        {
            State_GetParallelWithLine();
            break;
        }
        case ovrsPassSafetyCar:
        {
            State_PassSafetyCar();
            break;
        }
        case ovrsTurnBackToLine:
        {
            State_TurnBackToLine();
            break;
        }
        case ovrsSearchLine:
        {
            State_SearchLine();
            break;
        }
        case ovrsSuccessfulOvertake:
        {
            State_SuccessfulOvertake();
            break;
        }
        case ovrsFailedOvertake:
        {
            State_FailedOvertake();
            break;
        }
        default:
        {
            break;
        }
    }
}

void Overtake::State_Prepare()
{
    control_values.speed       = OVERTAKE_PREPARE_SPEED;
    control_values.wheel_angle = OVERTAKE_PREPARE_WHEEL_ANGLE;
    control_values.error       = ovrerrOngoingOvertake;
    // TODO get yaw

    if (dist_waiter->IsExpired())
    {
        dist_waiter->Wait_m(OVERTAKE_LEAVE_LINE_WAIT_DIST);

        state = ovrsLeaveLine;

#ifdef OVERTAKE_TRACE_ENABLED
        PRINTF("Overtake: Turn left to leave line.\n");
#endif
    }
}

void Overtake::State_LeaveLine()
{
    control_values.speed       = OVERTAKE_LEAVE_LINE_SPEED;
    control_values.wheel_angle = OVERTAKE_LEAVE_LINE_WHEEL_ANGLE;
    control_values.error       = ovrerrOngoingOvertake;

    if (dist_waiter->IsExpired())
    {
        dist_waiter->Wait_m(OVERTAKE_GET_PARALLEL_WAIT_DIST);

        state = ovrsGetBackParallel;

#ifdef OVERTAKE_TRACE_ENABLED
        PRINTF("Overtake: Turn right to get parallel with the safety car.\n");
#endif
    }
}

void Overtake::State_GetParallelWithLine()
{
    control_values.speed       = OVERTAKE_GET_PARALLEL_SPEED;
    control_values.wheel_angle = OVERTAKE_GET_PARALLEL_WHEEL_ANGLE;
    control_values.error       = ovrerrOngoingOvertake;

    if (dist_waiter->IsExpired())
    {
        dist_waiter->Wait_m(OVERTAKE_PASS_CAR_WAIT_DIST);

        state = ovrsPassSafetyCar;

#ifdef OVERTAKE_TRACE_ENABLED
        PRINTF("Overtake: Pass the safety car.\n");
#endif
    }
}

void Overtake::State_PassSafetyCar()
{
    control_values.speed       = OVERTAKE_PASS_CAR_SPEED;
    control_values.wheel_angle = OVERTAKE_PASS_CAR_WHEEL_ANGLE;
    control_values.error       = ovrerrOngoingOvertake;
    // TODO get yaw and control wheel angle

    if (dist_waiter->IsExpired())
    {
        dist_waiter->Wait_m(OVERTAKE_TURN_TO_LINE_WAIT_DIST);

        state = ovrsTurnBackToLine;

#ifdef OVERTAKE_TRACE_ENABLED
        PRINTF("Overtake: Turn right to approach the line.\n");
#endif
    }
}

void Overtake::State_TurnBackToLine()
{
    control_values.speed       = OVERTAKE_TURN_TO_LINE_SPEED;
    control_values.wheel_angle = OVERTAKE_TURN_TO_LINE_WHEEL_ANGLE;
    control_values.error       = ovrerrOngoingOvertake;

    if (dist_waiter->IsExpired())
    {
        dist_waiter->Wait_m(OVERTAKE_SEARCH_LINE_WAIT_DIST);

        state = ovrsSearchLine;

#ifdef OVERTAKE_TRACE_ENABLED
        PRINTF("Overtake: Turn left catch the line.\n");
#endif
    }
}

void Overtake::State_SearchLine()
{
    control_values.speed       = OVERTAKE_SEARCH_LINE_SPEED;
    control_values.wheel_angle = OVERTAKE_SEARCH_LINE_WHEEL_ANGLE;
    control_values.error       = ovrerrOngoingOvertake;

    if (line_detector->GetTrackType() != None)
    {
#ifdef OVERTAKE_TRACE_ENABLED
    PRINTF("Overtake: Successfully overtook the safety car.\n");
#endif
        state = ovrsSuccessfulOvertake;
    }

    if (dist_waiter->IsExpired())
    {
#ifdef OVERTAKE_TRACE_ENABLED
    PRINTF("Overtake: Failed to overtook the safety car.\n");
#endif
        state = ovrsFailedOvertake;
    }
}

void Overtake::State_SuccessfulOvertake()
{
    control_values.error = ovrerrSuccessfulOvertake;
}

void Overtake::State_FailedOvertake()
{
    control_values.error = ovrerrFailedOvertake;
    control_values.speed = 0.0f;
}
