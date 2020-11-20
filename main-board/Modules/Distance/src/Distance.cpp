#include "Distance.h"

#define TOF_POWER_EN_Pin       GPIO_PIN_14
#define TOF_POWER_EN_Port      GPIOC

Distance* Distance::GetInstance()
{
    static Distance instance;
    return &instance;
}

float Distance::GetDistance(DistanceSensor sensor)
{
    float distance = 0.0f;

    switch (sensor)
    {
        case ToF_Front:
        {
            distance = (float)(tof_front->GetDistance_mm()) / 1000.0f;
            break;
        }
        default:
        {
            break;
        }
    }
    return distance;
}

void Distance::SetFrontServo(float angle)
{
	angle *= 0.8f; // Magic value measured during calibration

	float offset = 3.14159265358979323846f / 2.0f - 0.0389f;
    float servo_angle = angle + offset;

    srv_front->SetSteerAngle(servo_angle);
}

void Distance::Process()
{
    tof_front->Process();
}

Distance::Distance()
{
	InitPower();

	srv_front = new Servo(eTIM8, TIM_CHANNEL_1);
    srv_front->Enable();

    tof_front = new TOF_L1(0x20,
                           100,
                           200,
                           TOF_FRONT_XSDN_Port,
                           TOF_FRONT_XSDN_Pin);
    tof_front->Init();

    SetFrontServo(1.57f);
}

void Distance::InitPower()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// GPIO Ports Clock Enable
	__HAL_RCC_GPIOC_CLK_ENABLE();

	// Configure GPIO pin Output Level
	HAL_GPIO_WritePin(GPIOC, TOF_POWER_EN_Pin, GPIO_PIN_RESET);

	// Configure GPIO pins : PCPin PCPin PCPin PCPin
	GPIO_InitStruct.Pin     = TOF_POWER_EN_Pin;
	GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull    = GPIO_NOPULL;
	GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOC, TOF_POWER_EN_Pin, GPIO_PIN_SET);
}

void PowerEnable(bool en)
{
	HAL_GPIO_WritePin(GPIOC, TOF_POWER_EN_Pin, en ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
