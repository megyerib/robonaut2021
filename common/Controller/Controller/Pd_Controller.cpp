#include "Pd_Controller.h"

Pd_Controller::Pd_Controller(float Kp, float Kd)
{
    kp = Kp;
    kd = Kd;
}

void Pd_Controller::Process(float y) // TODO revise
{
	float err  = y - ref;
    derivative = err - err_prev;
    err_prev   = err;

    cout = - (err * kp) - (derivative * kd);
}
