#pragma once

#include "Controller.h"

class Pd_Controller : public Controller
{
private:
    float kp;
	float kd;

    float derivative     = 0;
    float previous_error = 0;
    float err_prev       = 0;

public:
    Pd_Controller(float Kp, float Kd);

    virtual void Process(float y) override;

    void SetPD(float P, float D) {kp = P; kd = D;}
};
