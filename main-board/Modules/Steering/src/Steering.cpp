#include "Steering.h"
#include "SerialMessages.h"
#include "Trace.h"
#include "Uptime.h"

#define PI 3.1415926535f

#define SINGLE_SLOW_P   ( 15.0f)
#define SINGLE_SLOW_D   (200.0f)

#define SINGLE_FAST_P   ( 20.0f)
#define SINGLE_FAST_D   (200.0f)

#define SINGLE_RACE_TURN_P      ( 19.0f)    // 20
#define SINGLE_RACE_TURN_D      (190.0f)    // 200

#define SINGLE_RACE_ACCEL_P     ( 19.0f)
#define SINGLE_RACE_ACCEL_D     (190.0f)

#define SINGLE_RACE_STRAIGHT_P  ( 10.0f)    // 10
#define SINGLE_RACE_STRAIGHT_D  (100.0f)    // 100

#define SINGLE_RACE_DECEL_P     ( 19.0f)
#define SINGLE_RACE_DECEL_D     (190.0f)


Steering::Steering()
{
	InitEnablePin();

	front.servo      = new Servo(srvFront);
	front.controller = new Pd_Controller(0, 0); // Set P and D in the macros
	front.line       = 0.0f;
	front.angle      = 0.0f;

	mode = SteeringMode::Manual;
}

Steering* Steering::GetInstance()
{
    static Steering instance;
    return &instance;
}

void Steering::SetMode(SteeringMode mode)
{
    this->mode = mode;

    switch (mode)
    {
        case SingleLineFollow_Slow:
        {
            front.controller->Set_P_Term(SINGLE_SLOW_P);
            front.controller->Set_D_Term(SINGLE_SLOW_D);
            break;
        }
        case SingleLineFollow_Fast:
        {
            front.controller->Set_P_Term(SINGLE_FAST_P);
            front.controller->Set_D_Term(SINGLE_FAST_D);
            break;
        }
        case SingleLine_Race_Turn:
        {
            front.controller->Set_P_Term(SINGLE_RACE_TURN_P);
            front.controller->Set_D_Term(SINGLE_RACE_TURN_D);
            break;
        }
        case SingleLine_Race_Accel:
        {
            front.controller->Set_P_Term(SINGLE_RACE_ACCEL_P);
            front.controller->Set_D_Term(SINGLE_RACE_ACCEL_D);
            break;
        }
        case SingleLine_Race_Straight:
        {
            front.controller->Set_P_Term(SINGLE_RACE_STRAIGHT_P);
            front.controller->Set_D_Term(SINGLE_RACE_STRAIGHT_D);
            break;
        }
        case SingleLine_Race_Decel:
        {
            front.controller->Set_P_Term(SINGLE_RACE_DECEL_P);
            front.controller->Set_D_Term(SINGLE_RACE_DECEL_D);
            break;
        }
        default:
        {

            break;
        }
    }
}

void Steering::SetLine(float front_line, float rear_line) // TODO add line with angle
{
    front.line = front_line;
}

//! Steering wheel:
//!
//!                 0 rad
//!        0.52 rad   |   -0.52 rad
//!               \   |   /
//!                \  |  /
//!                 \ | /
//!  1.57 rad________\|/________-1.57 rad
//!  Left end                   Right end
void Steering::SetAngleManual(float front_angle)
{
	//SetFrontAngle(front_angle);
	//SetRearAngle(rear_angle);

	front.angle = front_angle;
}

void Steering::Process()
{
    switch (mode)
    {
    	case SingleLineFollow_Slow:
    	case SingleLineFollow_Fast:
    	case SingleLine_Race_Straight:
    	case SingleLine_Race_Decel:
    	case SingleLine_Race_Accel:
    	case SingleLine_Race_Turn:
        {
            front.controller->Process(front.line);

            float angle = front.controller->GetControlValue();

            SetFrontAngle(angle);
            break;
        }
        case Manual:
        {
        	SetFrontAngle(front.angle);

            break;
        }
        default:
        	break;
    }
}

void Steering::SetFrontAngle(float angle /* rad */)
{
	float offset        = PI/2.0f;
	float scale         = 1.0f;
	float servo_angle   = (angle + offset) * scale;

	front.servo->SetSteerAngle(servo_angle);
}

float Steering::GetFrontAngle()
{
    float angle;
    float offset        = PI/2.0f;

    angle = front.servo->GetSteerAngle() - offset;

    return angle;
}

void Steering::InitEnablePin()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitStruct.Pin    = GPIO_PIN_0;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
