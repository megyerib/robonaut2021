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

EXIT_INFO MazeExit::MazeExit_Process_Q2()
{
    if (error_code == memerrNoError)
    {
        switch (mode)
        {
            case memQ2Left:
            {
                StateMachine_Q2ExitLeft();
                break;
            }
            case memQ2Right:
            {
                StateMachine_Q2ExitRight();
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

EXIT_INFO MazeExit::Exit_Process()
{
    if (error_code == memerrNoError)
    {
        switch (mode)
        {
            case memQ2Left:
            {
                // TODO
                error_code = memerrUnknown;
                break;
            }
            case memQ2Right:
            {
                // TODO
                error_code = memerrUnknown;
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

void MazeExit::StateMachine_Q2ExitLeft()
{
    switch (state)
    {
        case mesInitialized:
        {
            dist_waiter.Wait_m(Q2_LEFTEXIT_PREPARE_WAIT_DIST);

            control_values.speed       = Q2_LEFTEXIT_PREPARE_SPEED;
            control_values.wheel_angle = Q2_LEFTEXIT_PREPARE_WHEEL_ANGLE;
            control_values.error       = memerrOngoingExitLeftQ2;

            state = mesQ2LeftExit_Prepare;
            break;
        }
        case mesQ2LeftExit_Prepare:
        {
            StateQ2_ExitLeft_PrepareToLeaveLine();
            break;
        }
        case mesQ2LeftExit_LeaveLine:
        {
            StateQ2_ExitLeft_LeaveLine();
            break;
        }
        case mesQ2LeftExit_SearchLine:
        {
            StateQ2_ExitLeft_SearchLine();
            break;
        }
        default:
        {
            break;
        }
    }
}

void MazeExit::StateQ2_ExitLeft_PrepareToLeaveLine()
{
    control_values.speed       = Q2_LEFTEXIT_PREPARE_SPEED;
    control_values.wheel_angle = Q2_LEFTEXIT_PREPARE_WHEEL_ANGLE;
    control_values.error       = memerrOngoingExitLeftQ2;

    if (dist_waiter.IsExpired())
    {
        dist_waiter.Wait_m(Q2_LEFTEXIT_LEAVE_LINE_WAIT_DIST);

        state = mesQ2LeftExit_LeaveLine;
    }
}

void MazeExit::StateQ2_ExitLeft_LeaveLine()
{
    control_values.speed       = Q2_LEFTEXIT_LEAVE_LINE_SPEED;
    control_values.wheel_angle = Q2_LEFTEXIT_LEAVE_LINE_WHEEL_ANGLE;
    control_values.error       = memerrOngoingExitLeftQ2;

    if (dist_waiter.IsExpired() && (line_detector->GetTrackType() == None))
    {
        dist_waiter.Wait_m(Q2_LEFTEXIT_SEARCH_LINE_WAIT_DIST);

        state = mesQ2LeftExit_SearchLine;
    }
    else if (dist_waiter.GetDiff() > Q2_LEFTEXIT_LEAVE_LINE_MAX_DISTANCE)
    {
        state = mesFailedToExit;
    }
    else
    {
        // Waiting.
    }
}

void MazeExit::StateQ2_ExitLeft_SearchLine()
{
    control_values.speed       = Q2_LEFTEXIT_SEARCH_LINE_SPEED;
    control_values.wheel_angle = Q2_LEFTEXIT_SEARCH_LINE_WHEEL_ANGLE;
    control_values.error       = memerrOngoingExitLeftQ2;

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

void MazeExit::StateMachine_Q2ExitRight()
{
    switch (state)
    {
        case mesInitialized:
        {
            dist_waiter.Wait_m(Q2_RIGHTEXIT_PREPARE_WAIT_DIST);

            control_values.speed       = Q2_RIGHTEXIT_PREPARE_SPEED;
            control_values.wheel_angle = Q2_RIGHTEXIT_PREPARE_WHEEL_ANGLE;
            control_values.error       = memerrOngoingExitLeftQ2;

            state = mesQ2RightExit_Prepare;
            break;
        }
        case mesQ2RightExit_Prepare:
        {
            StateQ2_ExitRight_PrepareToLeaveLine();
            break;
        }
        case mesQ2RightExit_LeaveLine:
        {
            StateQ2_ExitLeft_LeaveLine();
            break;
        }
        case mesQ2RightExit_Reverse:
        {
            StateQ2_ExitRight_Reverse();
            break;
        }
        case mesQ2RightExit_SearchLine:
        {
            StateQ2_ExitRight_SearchLine();
            break;
        }
        default:
        {
            break;
        }
    }
}

void MazeExit::StateQ2_ExitRight_PrepareToLeaveLine()
{
    control_values.speed       = Q2_RIGHTEXIT_PREPARE_SPEED;
    control_values.wheel_angle = Q2_RIGHTEXIT_PREPARE_WHEEL_ANGLE;
    control_values.error       = memerrOngoingExitRightQ2;

    if (dist_waiter.IsExpired())
    {
        dist_waiter.Wait_m(Q2_RIGHTEXIT_LEAVE_LINE_WAIT_DIST);

        state = mesQ2RightExit_LeaveLine;
    }
}

void MazeExit::StateQ2_ExitRight_LeaveLine()
{
    control_values.speed       = Q2_RIGHTEXIT_LEAVE_LINE_SPEED;
    control_values.wheel_angle = Q2_RIGHTEXIT_LEAVE_LINE_WHEEL_ANGLE;
    control_values.error       = memerrOngoingExitRightQ2;

    if (dist_waiter.IsExpired())
    {
        dist_waiter.Wait_m(Q2_RIGHTEXIT_REVERSE_WAIT_DIST);

        state = mesQ2RightExit_Reverse;
    }
}

void MazeExit::StateQ2_ExitRight_Reverse()
{
    control_values.speed       = Q2_RIGHTEXIT_REVERSE_SPEED;
    control_values.wheel_angle = Q2_RIGHTEXIT_REVERSE_WHEEL_ANGLE;
    control_values.error       = memerrOngoingExitRightQ2;

    if (dist_waiter.IsExpired())
    {
        dist_waiter.Wait_m(Q2_RIGHTEXIT_SEARCH_LINE_WAIT_DIST);

        state = mesQ2RightExit_SearchLine;
    }
}

void MazeExit::StateQ2_ExitRight_SearchLine()
{
    control_values.speed       = Q2_RIGHTEXIT_SEARCH_LINE_SPEED;
    control_values.wheel_angle = Q2_RIGHTEXIT_SEARCH_LINE_WHEEL_ANGLE;
    control_values.error       = memerrOngoingExitRightQ2;

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

void StateMachine()
{
    // TODO implement
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

