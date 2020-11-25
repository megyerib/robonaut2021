#pragma once

#include "Controller.h"

class Pd_Controller : public Controller
{
private:
    float kd;
    float derivative;
    float previous_error;
    float previous_line;

public:
    Pd_Controller();
    Pd_Controller(float const Kp, float const Kd);

    void Process(float const processValue) override;

    void Set_D_Term(float const D);
};
