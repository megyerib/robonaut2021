#pragma once

struct FeedbackLoop
{
    float setpoint;
    float error_value;
    float control_value;
    float process_value;

    void Init();
};

class Controller
{
protected:
    FeedbackLoop loop;
    float        kp;

    void CalculateErrorValue(float processValue);

public:
    Controller();

    virtual void Process(float processValue);

    float GetControlValue();
    void  SetSetpoint(float SP);
    void  Set_P_Term(float P);

    void SaturateControlValue(float min, float max);
};

