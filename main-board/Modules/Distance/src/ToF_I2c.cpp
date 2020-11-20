#include "Tof_I2c.h"
#include "vl53l1_platform.h"

ToF_I2c* ToF_I2c::GetInstance()
{
    static ToF_I2c instance;
    return &instance;
}

I2C_HandleTypeDef* ToF_I2c::GetHandle()
{
    return handle;
}

void ToF_I2c::Reset()
{
    DeInit();
    Init();
}

ToF_I2c::ToF_I2c()
{
    Init();
}

void ToF_I2c::Init()
{
    handle = &tof_handle;

    ConfigureGpio();

    // I2C1 clock enable
    __HAL_RCC_I2C1_CLK_ENABLE();

    // I2C1 interrupt Init
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);

    ConfigureHandle();
}

void ToF_I2c::ConfigureHandle()
{
    handle->Instance                 = I2C1;
    handle->Init.ClockSpeed          = 100000;
    handle->Init.DutyCycle           = I2C_DUTYCYCLE_2;
    handle->Init.OwnAddress1         = 0;
    handle->Init.AddressingMode      = I2C_ADDRESSINGMODE_7BIT;
    handle->Init.DualAddressMode     = I2C_DUALADDRESS_DISABLE;
    handle->Init.OwnAddress2         = 0;
    handle->Init.GeneralCallMode     = I2C_GENERALCALL_DISABLE;
    handle->Init.NoStretchMode       = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(handle) != HAL_OK)
    {
        //Error_Handler();
    }
}

void ToF_I2c::ConfigureGpio()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    // I2C1 GPIO Configuration:
    //   PB6 ------> I2C1_SCL
    //   PB7 ------> I2C1_SDA
    //
    GPIO_InitStruct.Pin         = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode        = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull        = GPIO_PULLUP;
    GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate   = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Init Debug GPIOSs (Free123)
    GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin         = GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3;
    GPIO_InitStruct.Mode        = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull        = GPIO_NOPULL;
    GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void ToF_I2c::DeInit()
{
    SET_BIT(handle->Instance->CR1,   I2C_CR1_SWRST);
    HAL_Delay(2);
    CLEAR_BIT(handle->Instance->CR1, I2C_CR1_SWRST);

    __HAL_RCC_I2C1_CLK_DISABLE();

    HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_7);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3);

    HAL_I2C_DeInit(handle);

    handle = {0};
    tof_handle = {0};
}
