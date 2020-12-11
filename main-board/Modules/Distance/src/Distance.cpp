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
	if (!InitDone) return 0;

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
	if (!InitDone) return;

	angle *= 0.8f; // Magic value measured during calibration

	float offset = 1.531896f;
    float servo_angle = angle + offset;

    srv_front->SetSteerAngle(servo_angle);
}

void Distance::Process()
{
    // Initialization includes waiting so it shall be done in the periodic process function

	if (!InitDone)
    {
		InitPower();
		InitServo();
		InitTof(); // Includes waiting

		InitDone = true;
    }
    else
    {
    	tof_front->Process();
    }
}

Distance::Distance()
{

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

void Distance::InitServo()
{
	srv_front = new Servo(eTIM8, TIM_CHANNEL_1);
}

void Distance::InitTof()
{
	tof_front = new TOF_L1(0x20,
						   100,
						   200,
						   TOF_FRONT_XSDN_Port,
						   TOF_FRONT_XSDN_Pin);
	tof_front->Init();

	SetFrontServo(0.0f);
}

void Distance::PowerEnable(bool en)
{
	HAL_GPIO_WritePin(GPIOC, TOF_POWER_EN_Pin, en ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
