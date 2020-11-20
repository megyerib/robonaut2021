#pragma once

#include "stm32f4xx_hal.h"

enum TimerInstance
{
    eTIM8 = 0,
    eTIM12
};

enum ServoType
{
    UNKNOWN = 0,
    DIGITAL, // D1 CH6012
    ANALOG   // Maverick MS-22
};

struct TimerConfig
{
    uint16_t Prescaler;
    uint16_t Period;
    uint8_t  Frequency;
    uint16_t TimerClk;
};

struct ServoConfig
{
    uint16_t    Rad_min;
    uint16_t    Rad_30deg;
    uint16_t    Rad_90deg;
    uint16_t    Rad_150deg;
    uint16_t    Rad_max;
    float       Gradient;
    float       Y_intercept;
};

class Servo
{
private:
     ServoType          type;
     bool               enabled;
     ServoConfig        config;
     TimerConfig        timCfg;
     TIM_HandleTypeDef* htim;
     uint32_t           channel;

     uint32_t SaturateCompareValue(uint32_t compare);
     void     SetTimerCompareValue(uint32_t compareValue);
     uint32_t GetTimerCompareValue(void);
     void Init();
     void InitTimer();
     void InitGPIOs();
     void LoadCfg();

public:
     Servo(TimerInstance TIM, uint32_t Channel);

     void  Enable(void);
     void  Disable(void);
     void  SetSteerAngle(float rad);
     float GetSteerAngle(void);

     void SetCalibration(uint8_t min,
                         uint8_t rad30,
                         uint8_t rad90,
                         uint8_t rad150,
                         uint8_t max);
};
