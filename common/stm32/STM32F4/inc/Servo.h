#pragma once

#include "stm32f4xx_hal.h"

// Front steering:   TIM12 CH2 (APB1)
// Rear steering:    TIM12 CH1 (APB1)
// Distance sensor:  TIM8  CH1 (APB2)

typedef enum
{
    eTIM8 = 0,
    eTIM12
}
TimerInstance;

typedef enum
{
    UNKNOWN = 0,
    DIGITAL, // D1 CH6012
    ANALOG   // Maverick MS-22
}
ServoType;

typedef struct
{
    uint16_t Prescaler;
    uint16_t Period;
    uint8_t  Frequency;
    uint16_t TimerClk;
}
TimerConfig;

typedef struct
{
    uint16_t    Rad_min;
    uint16_t    Rad_30deg;
    uint16_t    Rad_90deg;
    uint16_t    Rad_150deg;
    uint16_t    Rad_max;
    float       Gradient;
    float       Y_intercept;
}
ServoConfig;

class Servo
{
private:
     ServoType          type;
     ServoConfig        config;
     TimerConfig        timCfg;
     TIM_HandleTypeDef* htim;
     uint32_t           channel;

     uint32_t GetTimerCompareValue(void);

     void Init();
     void InitTimer();
     void InitGPIOs();
     void LoadCfg();

public:
     Servo(TimerInstance TIM, uint32_t Channel); // Channel must be sg like TIM_CHANNEL_1

     void  SetSteerAngle(float rad);
     float GetSteerAngle(void);

     void SetPulseWidth_us(uint32_t compareValue); // TODO make it private again

     void SetCalibration(uint8_t min,
                         uint8_t rad30,
                         uint8_t rad90,
                         uint8_t rad150,
                         uint8_t max);
};
