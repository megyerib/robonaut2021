#include "Pd_Controller.h"

#define INTUITIVE_CONSTANT  (-0.75f)    /* Cntrol not for the edge of the line sensor */

Pd_Controller::Pd_Controller(void)
{
    Controller();
    kd             = 0.0f;
    derivative     = 0.0f;
    previous_error = 0.0f;
}

Pd_Controller::Pd_Controller(float const Kp, float const Kd)
{
    Pd_Controller();
    kp = Kp;
    kd = Kd;
}

void Pd_Controller::Process(float const processValue)
{
    CalculateErrorValue(processValue);

    derivative = loop.process_value - previous_line;

    loop.control_value = INTUITIVE_CONSTANT * (loop.process_value * kp + derivative * kd);

    previous_line = loop.process_value;
//    // e(t)
//    CalculateErrorValue(processValue);
//
//    // d/dt e(t)
//    derivative = loop.error_value - previous_error;
//
//    // u(t) = P * e(t) + D * d/dt e(t)
//    loop.control_value = kp * loop.error_value + kd * derivative;
//
//    // Actualize.
//    previous_error = loop.error_value;
}

void Pd_Controller::Set_D_Term(float const D)
{
    kd = D;
}
