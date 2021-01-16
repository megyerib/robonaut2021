#include "MazeExit.h"
#include "MazeExit_Settings.h"

MazeExit& MazeExit::GetInstance()
{
    static MazeExit instance;
    return instance;
}

MEM_ERROR MazeExit::Init(MAZE_EXIT_MODE const mode)
{
    this->mode     = mode;
    error_code     = memerrNoError;
    state          = mesInitialized;
    control_values = {0.0f, 0.0f, memerrUnknown};
    initialized    = true;

    line_detector->SetMode(trackMaze);

    return error_code;
}

EXIT_INFO MazeExit::MazeExit_Process()
{
    if (error_code == memerrNoError)
    {
        switch (mode)
        {
            case memForward:
            {
                StateMachine_ExitForward();
                break;
            }
            case memY:
            {
                StateMachine_ExitY();
                break;
            }
            default:
            {
                error_code = memerrUnknown;
                break;
            }
        }
    }
    control_values.error = error_code;

    return control_values;
}

MazeExit::MazeExit()
{
    mode           = memNotSet;
    error_code     = memerrNoError;
    state          = mesUninitialzed;
    control_values = {0.0f, 0.0f, memerrUnknown};
    initialized    = false;
    line_detector  = TrackDetector::GetInstance();
}

void MazeExit::StateMachine_ExitForward()
{
    switch (state)
    {
        case mesInitialized:
        {
            dist_waiter.Wait_m(FORWARDEXIT_PREPARE_WAIT_DIST);

            control_values.speed       = FORWARDEXIT_PREPARE_SPEED;
            control_values.wheel_angle = FORWARDEXIT_PREPARE_WHEEL_ANGLE;
            control_values.error       = memerrOngoingExitForward;

            state = mesForwardExit_Prepare;
            break;
        }
        case mesForwardExit_Prepare:
        {
            State_ExitForward_PrepareToLeaveLine();
            break;
        }
        case mesForwardExit_LeaveLine:
        {
            State_ExitForward_LeaveLine();
            break;
        }
        case mesForwardExit_SearchLine:
        {
            State_ExitForward_SearchLine();
            break;
        }
        default:
        {
            break;
        }
    }
}

void MazeExit::State_ExitForward_PrepareToLeaveLine()
{
    control_values.speed       = FORWARDEXIT_PREPARE_SPEED;
    control_values.wheel_angle = FORWARDEXIT_PREPARE_WHEEL_ANGLE;
    control_values.error       = memerrOngoingExitForward;

    if (dist_waiter.IsExpired())
    {
        dist_waiter.Wait_m(FORWARDEXIT_LEAVE_LINE_WAIT_DIST);

        state = mesForwardExit_LeaveLine;
    }
}

void MazeExit::State_ExitForward_LeaveLine()
{
    control_values.speed       = FORWARDEXIT_LEAVE_LINE_SPEED;
    control_values.wheel_angle = FORWARDEXIT_LEAVE_LINE_WHEEL_ANGLE;
    control_values.error       = memerrOngoingExitForward;

    if (dist_waiter.IsExpired())
    {
        dist_waiter.Wait_m(FORWARDEXIT_SEARCH_LINE_WAIT_DIST);

        state = mesForwardExit_SearchLine;
    }
}

void MazeExit::State_ExitForward_SearchLine()
{
    control_values.speed       = FORWARDEXIT_SEARCH_LINE_SPEED;
    control_values.wheel_angle = FORWARDEXIT_SEARCH_LINE_WHEEL_ANGLE;
    control_values.error       = memerrOngoingExitForward;

    if (line_detector->GetTrackType() != None && !dist_waiter.IsExpired())
    {
        State_ExitSuccessful();
        state = mesSuccessfullyFinished;
    }
    else
    {
        State_ExitFailed();
        state = mesFailedToExit;
    }
}

void MazeExit::StateMachine_ExitY()
{
    switch (state)
    {
        case mesInitialized:
        {
            dist_waiter.Wait_m(YEXIT_PREPARE_WAIT_DIST);

            control_values.speed       = YEXIT_PREPARE_SPEED;
            control_values.wheel_angle = YEXIT_PREPARE_WHEEL_ANGLE;
            control_values.error       = memerrOngoingExitForward;

            state = mesYExit_Prepare;
            break;
        }
        case mesYExit_Prepare:
        {
            State_ExitY_PrepareToLeaveLine();
            break;
        }
        case mesYExit_LeaveLine:
        {
            State_ExitForward_LeaveLine();
            break;
        }
        case mesYExit_Reverse:
        {
            State_ExitY_Reverse();
            break;
        }
        case mesYExit_SearchLine:
        {
            State_ExitY_SearchLine();
            break;
        }
        default:
        {
            break;
        }
    }
}

void MazeExit::State_ExitY_PrepareToLeaveLine()
{
    control_values.speed       = YEXIT_PREPARE_SPEED;
    control_values.wheel_angle = YEXIT_PREPARE_WHEEL_ANGLE;
    control_values.error       = memerrOngoingExitY;

    if (dist_waiter.IsExpired())
    {
        dist_waiter.Wait_m(YEXIT_LEAVE_LINE_WAIT_DIST);

        state = mesYExit_LeaveLine;
    }
}

void MazeExit::State_ExitY_LeaveLine()
{
    control_values.speed       = YEXIT_LEAVE_LINE_SPEED;
    control_values.wheel_angle = YEXIT_LEAVE_LINE_WHEEL_ANGLE;
    control_values.error       = memerrOngoingExitY;

    if (dist_waiter.IsExpired())
    {
        dist_waiter.Wait_m(YEXIT_REVERSE_WAIT_DIST);

        state = mesYExit_Reverse;
    }
}

void MazeExit::State_ExitY_Reverse()
{
    control_values.speed       = YEXIT_REVERSE_SPEED;
    control_values.wheel_angle = YEXIT_REVERSE_WHEEL_ANGLE;
    control_values.error       = memerrOngoingExitY;

    if (dist_waiter.IsExpired())
    {
        dist_waiter.Wait_m(YEXIT_SEARCH_LINE_WAIT_DIST);

        state = mesYExit_SearchLine;
    }
}

void MazeExit::State_ExitY_SearchLine()
{
    control_values.speed       = YEXIT_SEARCH_LINE_SPEED;
    control_values.wheel_angle = YEXIT_SEARCH_LINE_WHEEL_ANGLE;
    control_values.error       = memerrOngoingExitY;

    if ((line_detector->GetTrackType() != None) && !dist_waiter.IsExpired())
    {
        State_ExitSuccessful();
        state = mesSuccessfullyFinished;
    }
    else
    {
        State_ExitFailed();
        state = mesFailedToExit;
    }
}

void MazeExit::State_ExitSuccessful()
{
    control_values.error = memerrSuccessfullyEnded;
}

void MazeExit::State_ExitFailed()
{
    control_values.error = memerrFailedExit;
    control_values.speed = 0.0f;
}

