#include "Inert_I2c.h"

#include "lsm6dso_reg.h"

I2C_HandleTypeDef INERT_I2C::hi2c = {0};
bool INERT_I2C::configured = false;

INERT_I2C* INERT_I2C::GetInstance()
{
    static INERT_I2C instance;
    return &instance;
}

I2C_HandleTypeDef* INERT_I2C::GetHandle()
{
    return &hi2c;
}

void INERT_I2C::Reset()
{
    DeInit();
    Init();
}

INERT_I2C::INERT_I2C()
{
    if (configured == false)
    {
        Init();
    }
}

void INERT_I2C::Init()
{
    ConfigureGpio();

    // I2C3 clock enable
    __HAL_RCC_I2C3_CLK_ENABLE();

    // I2C3 interrupt Init
    HAL_NVIC_SetPriority(I2C3_EV_IRQn, 6, 1);
    HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
    HAL_NVIC_SetPriority(I2C3_ER_IRQn, 6, 1);
    HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);

    ConfigureHandle();

    configured = true;
}

void INERT_I2C::ConfigureHandle()
{
    hi2c.Instance                 = I2C3;
    hi2c.Init.ClockSpeed          = 100000;
    hi2c.Init.DutyCycle           = I2C_DUTYCYCLE_2;
    hi2c.Init.OwnAddress1         = 0;
    hi2c.Init.AddressingMode      = I2C_ADDRESSINGMODE_7BIT;
    hi2c.Init.DualAddressMode     = I2C_DUALADDRESS_DISABLE;
    hi2c.Init.OwnAddress2         = 0;
    hi2c.Init.GeneralCallMode     = I2C_GENERALCALL_DISABLE;
    hi2c.Init.NoStretchMode       = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c) != HAL_OK)
    {
        //Error_Handler();
    }
}

void INERT_I2C::ConfigureGpio()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // I2C3 GPIO Configuration:
    //  PC9     ------> I2C3_SDA
    GPIO_InitStruct.Pin         = GPIO_PIN_9;
    GPIO_InitStruct.Mode        = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull        = GPIO_PULLUP;
    GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate   = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    //  PA8     ------> I2C3_SCL
    GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin         = GPIO_PIN_8;
    GPIO_InitStruct.Mode        = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull        = GPIO_PULLUP;
    GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate   = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void INERT_I2C::DeInit()
{
    SET_BIT(hi2c.Instance->CR1,   I2C_CR1_SWRST);
    HAL_Delay(2);
    CLEAR_BIT(hi2c.Instance->CR1, I2C_CR1_SWRST);

    __HAL_RCC_I2C3_CLK_DISABLE();

    HAL_NVIC_DisableIRQ(I2C3_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C3_ER_IRQn);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_9);
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8);

    HAL_I2C_DeInit(&hi2c);

    hi2c = {0};
}
