#include "stm32f4xx_hal_gpio.h"
#include "Servo.h"

#define PI (3.14159265f)

#define SERVO_FRONT_Pin         GPIO_PIN_14
#define SERVO_FRONT_GPIO_Port   GPIOB
#define SERVO_REAR_Pin          GPIO_PIN_15
#define SERVO_REAR_GPIO_Port    GPIOB
#define SERVO_SENSOR_Pin        GPIO_PIN_5
#define SERVO_SENSOR_GPIO_Port  GPIOA

static TIM_HandleTypeDef htim8;
static TIM_HandleTypeDef htim12;

Servo::Servo(TimerInstance TIM, uint32_t Channel)
{
    switch (TIM)
    {
        case eTIM8:
        {
            type = ANALOG;
            htim = &htim8;
            break;
        }
        case eTIM12:
        {
            type = DIGITAL;
            htim = &htim12;
            break;
        }
        default:
        {
            type = UNKNOWN;
            break;
        }
    }

    channel = Channel;

    Init();
}

void Servo::Enable(void)
{
    enabled = true;
}

void Servo::Disable(void)
{
    enabled = false;
}

//! Servo Anlge:
//!                 1.57 rad
//!          2.09 rad  |  0.52 rad
//!                 \  |  /
//!                  \ | /
//!    3.14 rad_______\|/________0 rad
//!    Left end                 Right end
void  Servo::SetSteerAngle(float rad)
{
    float compareValue;
    float validCompareValue;

    compareValue = (rad - config.Y_intercept) / config.Gradient;

    // Makes sure that the value is in the valid interval
    if(compareValue > config.Rad_max)
    {
        validCompareValue = config.Rad_max;
    }
    else if(compareValue < config.Rad_min)
    {
        validCompareValue = config.Rad_min;
    }
    else
    {
        validCompareValue = compareValue;
    }

    SetTimerCompareValue(validCompareValue);
}

float Servo::GetSteerAngle(void)
{
    float radian;
    float compareValue;

    compareValue = GetTimerCompareValue();
    radian = config.Gradient * compareValue + config.Y_intercept;

    return radian;
}

void Servo::SetCalibration(uint8_t min,
                           uint8_t rad30,
                           uint8_t rad90,
                           uint8_t rad150,
                           uint8_t max)
{
    config.Rad_min    = min;
    config.Rad_30deg  = rad30;
    config.Rad_90deg  = rad90;
    config.Rad_150deg = rad150;
    config.Rad_max    = max;
}

uint32_t Servo::SaturateCompareValue(uint32_t compare)
{
    uint32_t validCompareValue;

    if(compare > timCfg.Period)
    {
        validCompareValue = timCfg.Period;
    }
    else if(compare < 0)
    {
        validCompareValue = 0;
    }
    else
    {
        validCompareValue = compare;
    }

    return validCompareValue;
}

void Servo::SetTimerCompareValue(uint32_t compareValue)
{
    uint32_t validCompareValue = SaturateCompareValue(compareValue);

    __HAL_TIM_SET_COMPARE(htim, channel, validCompareValue);
}

uint32_t Servo::GetTimerCompareValue(void)
{
    return __HAL_TIM_GET_COMPARE(htim, channel);
}

void Servo::Init()
{
    if (type == DIGITAL)
    {
        /* TIM12 clock enable */
        __HAL_RCC_TIM12_CLK_ENABLE();

        /* TIM12 interrupt Init */
        HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
    }
    else if (type == ANALOG)
    {
        /* TIM8 clock enable */
        __HAL_RCC_TIM8_CLK_ENABLE();

        /* TIM8 interrupt Init */
        HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
        HAL_NVIC_SetPriority(TIM8_CC_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);
    }
    else
    {
        // Error.
    }

    InitTimer();
    InitGPIOs();

    enabled = false;

    LoadCfg();


    if (type == DIGITAL)
    {
         HAL_TIM_PWM_Start(htim, channel);
    }
    else
    {
        HAL_TIMEx_PWMN_Start(htim, channel);
    }
}

void Servo::InitTimer()
{
    if (type == DIGITAL)
    {
        TIM_ClockConfigTypeDef  sClockSourceConfig = {0};
        TIM_OC_InitTypeDef      sConfigOC = {0};

        htim->Instance                   = TIM12;
        htim->Init.Prescaler             = 1439;
        htim->Init.CounterMode           = TIM_COUNTERMODE_UP;
        htim->Init.Period                = 249;
        htim->Init.ClockDivision         = TIM_CLOCKDIVISION_DIV1;
        htim->Init.AutoReloadPreload     = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if (HAL_TIM_Base_Init(htim) != HAL_OK)
        {
            //Error_Handler();
        }
        sClockSourceConfig.ClockSource  = TIM_CLOCKSOURCE_INTERNAL;
        if (HAL_TIM_ConfigClockSource(htim, &sClockSourceConfig) != HAL_OK)
        {
            //Error_Handler();
        }
        if (HAL_TIM_PWM_Init(htim) != HAL_OK)
        {
            //Error_Handler();
        }
        sConfigOC.OCMode                = TIM_OCMODE_PWM1;
        sConfigOC.Pulse                 = 0;
        sConfigOC.OCPolarity            = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode            = TIM_OCFAST_DISABLE;
        if (channel == TIM_CHANNEL_1)
        {
            if (HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
            {
                //Error_Handler();
            }
        }
        if (channel == TIM_CHANNEL_2)
        {
            if (HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
            {
                //Error_Handler();
            }
        }
    }
    else if (type == ANALOG)
    {
        TIM_ClockConfigTypeDef          sClockSourceConfig = {0};
        TIM_MasterConfigTypeDef         sMasterConfig = {0};
        TIM_OC_InitTypeDef              sConfigOC = {0};
        TIM_BreakDeadTimeConfigTypeDef  sBreakDeadTimeConfig = {0};

        htim->Instance                  = TIM8;
        htim->Init.Prescaler            = 2879;
        htim->Init.CounterMode          = TIM_COUNTERMODE_UP;
        htim->Init.Period               = 1249;
        htim->Init.ClockDivision        = TIM_CLOCKDIVISION_DIV1;
        htim->Init.RepetitionCounter    = 0;
        htim->Init.AutoReloadPreload    = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if (HAL_TIM_Base_Init(htim) != HAL_OK)
        {
          //Error_Handler();
        }
        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        if (HAL_TIM_ConfigClockSource(htim, &sClockSourceConfig) != HAL_OK)
        {
          //Error_Handler();
        }
        if (HAL_TIM_PWM_Init(htim) != HAL_OK)
        {
          //Error_Handler();
        }
        sMasterConfig.MasterOutputTrigger   = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode       = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(htim, &sMasterConfig) != HAL_OK)
        {
          //Error_Handler();
        }
        sConfigOC.OCMode        = TIM_OCMODE_PWM1;
        sConfigOC.Pulse         = 0;
        sConfigOC.OCPolarity    = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCNPolarity   = TIM_OCNPOLARITY_HIGH;
        sConfigOC.OCFastMode    = TIM_OCFAST_DISABLE;
        sConfigOC.OCIdleState   = TIM_OCIDLESTATE_RESET;
        sConfigOC.OCNIdleState  = TIM_OCNIDLESTATE_RESET;
        if (HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
        {
          //Error_Handler();
        }
        sBreakDeadTimeConfig.OffStateRunMode    = TIM_OSSR_DISABLE;
        sBreakDeadTimeConfig.OffStateIDLEMode   = TIM_OSSI_DISABLE;
        sBreakDeadTimeConfig.LockLevel          = TIM_LOCKLEVEL_OFF;
        sBreakDeadTimeConfig.DeadTime           = 0;
        sBreakDeadTimeConfig.BreakState         = TIM_BREAK_DISABLE;
        sBreakDeadTimeConfig.BreakPolarity      = TIM_BREAKPOLARITY_HIGH;
        sBreakDeadTimeConfig.AutomaticOutput    = TIM_AUTOMATICOUTPUT_DISABLE;
        if (HAL_TIMEx_ConfigBreakDeadTime(htim, &sBreakDeadTimeConfig) != HAL_OK)
        {
          //Error_Handler();
        }
    }
    else
    {
        // Error.
    }
}

void Servo::InitGPIOs()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (type == DIGITAL)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**TIM12 GPIO Configuration
        PB14     ------> TIM12_CH1
        PB15     ------> TIM12_CH2
        */
        if (channel == TIM_CHANNEL_1)
        {
            GPIO_InitStruct.Pin     = SERVO_FRONT_Pin;
        }
        else if (TIM_CHANNEL_2)
        {
            GPIO_InitStruct.Pin     = SERVO_REAR_Pin;
        }
        else
        {
            // Error
        }
        GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull        = GPIO_NOPULL;
        GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate   = GPIO_AF9_TIM12;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
    else if (type == ANALOG)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**TIM8 GPIO Configuration
        PA5     ------> TIM8_CH1N
        */
        GPIO_InitStruct.Pin         = SERVO_SENSOR_Pin;
        GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull        = GPIO_NOPULL;
        GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate   = GPIO_AF3_TIM8;
        HAL_GPIO_Init(SERVO_SENSOR_GPIO_Port, &GPIO_InitStruct);
    }
    else
    {
        // Error.
    }
}

void Servo::LoadCfg()
{
    if (type == DIGITAL)
    {
        config.Rad_min      = 62;
        config.Rad_30deg    = 76;
        config.Rad_90deg    = 88;
        config.Rad_150deg   = 114;
        config.Rad_max      = 118;

        timCfg.Period       = 249;
        timCfg.Prescaler    = 1439;
    }
    else if (type == ANALOG)
    {
        config.Rad_min      = 49;
        config.Rad_30deg    = 49;
        config.Rad_90deg    = 85;
        config.Rad_150deg   = 121;
        config.Rad_max      = 121;

        timCfg.Period       = 1249;
        timCfg.Prescaler    = 2879;
    }
    else
    {
        // Error.
    }

    config.Gradient     = ((PI/180.0f) * (90.0f - 30.0f)/(config.Rad_90deg - config.Rad_30deg));
    config.Y_intercept  = ((PI/180.0f) * 90.0f - config.Rad_90deg * config.Gradient);
}
