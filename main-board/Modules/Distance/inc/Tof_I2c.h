#pragma once

#include "stm32f4xx_hal.h"

class ToF_I2c
{
public:
    static ToF_I2c* GetInstance();
    I2C_HandleTypeDef* GetHandle();

    void Reset();

private:
    I2C_HandleTypeDef* handle;

    ToF_I2c();
    void Init();
    void ConfigureHandle();
    void ConfigureGpio();
    void DeInit();
};
