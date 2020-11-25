#include "Controller.h"

void FeedbackLoop::Init()
{
    setpoint        = 0.0f;
    error_value     = 0.0f;
    control_value   = 0.0f;
    process_value   = 0.0f;
}

Controller::Controller(void)
{
    loop.Init();
    kp = 0.0f;
}

void Controller::Process(float processValue)
{
    // e(t) = y(t) - r(t)
    CalculateErrorValue(processValue);

    // u(t) = P * e(t)
    loop.control_value = loop.error_value * kp;
}

void Controller::CalculateErrorValue(float processValue)
{
    loop.process_value = processValue;

    loop.error_value = loop.setpoint - loop.process_value;
}

float Controller::GetControlValue(void)
{
    return loop.control_value;
}

void Controller::SetSetpoint(float SP)
{
    loop.setpoint = SP;
}

void Controller::Set_P_Term(float P)
{
    kp = P;
}

void Controller::SaturateControlValue(float min, float max)
{
    if (loop.control_value < min)
    {
        loop.control_value = min;
    }
    else if (loop.control_value > max)
    {
        loop.control_value = max;
    }
    else
    {
        // Control value is in valid range.
    }
}
