#pragma once

#include "Controller.h"

class Pid_Controller : public Controller
{
private:
    float ki;
    float kd;
    float integrate;
    float derivative;
    float previous_error;

    float integrate_limit;

public:
    Pid_Controller();
    Pid_Controller(float const Kp, float const Ki, float const Kd);

    void Process(float const porcessValue) override;

    void Set_I_Term(float const I);
    void Set_D_Term(float const D);

    void Reset();
    void Set_I_Limit(float const limit);
    float Get_Setpoint();

private:
    void CalcILimit(float const speed);
};
