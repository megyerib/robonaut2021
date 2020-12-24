#include "Steering.h"
#include "SerialMessages.h"
#include "Trace.h"
#include "Uptime.h"

#define PI 3.1415926535f

#define TURN_P      (  10.5f  )
#define TURN_D      (  75.0f  )

#define STRAIGHT_P  (   7.5f  )
#define STRAIGHT_D  (  75.0f  )

Steering::Steering()
{
	InitEnablePin();

	front.servo      = new Servo(srvFront);
	front.controller = new Pd_Controller(0, 0); // Set P and D in the macros
	front.line       = 0.0f;
	front.angle      = 0.0f;

	mode = SteeringMode::smManual;
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
        case smTurn:
        {
            front.controller->SetPD(TURN_P, TURN_D);
            break;
        }
        case smStraight:
        {
            front.controller->SetPD(STRAIGHT_P, STRAIGHT_D);
            break;
        }
        default:
        {
            break;
        }
    }
}

void Steering::SetLine(float front_line, float rear_line)
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
    	case smStraight:
    	case smTurn:
        {
            front.controller->Process(front.line);

            // Line:   - ... +
            // Angle:  +     -    ==> -1 multiplier
            //          \   /
            //           \ /

            float angle = front.controller->GetOutput();

            SetFrontAngle(angle);

            break;
        }
        case smManual:
        {
        	SetFrontAngle(front.angle);
            break;
        }
        default:
        {
        	break;
        }
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
