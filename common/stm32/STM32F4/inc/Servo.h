#pragma once

#include "stm32f4xx_hal.h"

// Front steering:   TIM12 CH2 (APB1)
// Rear steering:    TIM12 CH1 (APB1)
// Distance sensor:  TIM8  CH1 (APB2)

typedef enum
{
    srvFront, // D1 CH6012
	srvRear,  // D1 CH6012
    srvSensor // Maverick MS-22
}
ServoType;

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
public:
     Servo(ServoType t); // Channel must be sg like TIM_CHANNEL_1

     void  SetSteerAngle(float rad);
     float GetSteerAngle(void);

     void SetPulseWidth_us(uint32_t compareValue);

private:
     ServoType          type;
     ServoConfig        config;
     TIM_HandleTypeDef* htim;
     uint32_t           channel;

     static TIM_HandleTypeDef htim8;
     static TIM_HandleTypeDef htim12;

     void Init();
     void InitTimer();
     void InitGPIOs();
     void LoadCfg();
};
