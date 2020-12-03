#pragma once

#include "stm32f4xx_hal.h"

class INERT_I2C
{
public:
    static INERT_I2C* GetInstance();
    I2C_HandleTypeDef* GetHandle();

    void Reset();

private:
    static I2C_HandleTypeDef hi2c;
    static bool configured;

    INERT_I2C();
    void Init();
    void ConfigureHandle();
    void ConfigureGpio();
    void DeInit();
};
