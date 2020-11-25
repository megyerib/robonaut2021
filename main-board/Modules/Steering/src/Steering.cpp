#include "Steering.h"

#define PI 3.1415926535f

#define SINGLE_SLOW_P   ( 20.0f)
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

#define FRONT_OFFSET    ( 0.194f)
#define REAR_OFFSET     (-0.474f)


Steering::Steering()
{
	InitEnablePin();

	front.servo      = new Servo(eTIM12, TIM_CHANNEL_2);
	front.controller = new Pd_Controller(0.5f, 0.1f);
	front.line       = 0.0f;
	front.angle      = 0.0f;
	front.servo->Enable();

	rear.servo       = new Servo(eTIM12, TIM_CHANNEL_1);
	rear.controller  = new Pd_Controller(0.5f, 0.1f);
	rear.line        = 0.0f;
	rear.angle       = 0.0f;
	rear.servo->Enable();

	mode = SteeringMode::Off;
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
        case DualLineFollow_Slow:
        {
           rear.controller->Set_P_Term(SINGLE_SLOW_P);
           rear.controller->Set_D_Term(SINGLE_SLOW_D);
           front.controller->Set_P_Term(SINGLE_SLOW_P);
           front.controller->Set_D_Term(SINGLE_SLOW_D);
           break;
        }
        case SingleLineFollow_Slow:
        {
            front.controller->Set_P_Term(SINGLE_SLOW_P);
            front.controller->Set_D_Term(SINGLE_SLOW_D);
            break;
        }
        case DualLineFollow_Fast:
        case SingleLineFollow_Fast:
        {
            front.controller->Set_P_Term(SINGLE_FAST_P);
            front.controller->Set_D_Term(SINGLE_FAST_D);
            break;
        }
        case SingleLine_Race_Turn:
        case DualLine_Race_Turn:
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
        case DualLine_Race_Straight:
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

void Steering::SetLine(float front_line, float rear_line)
{
    front.line = front_line;
    rear.line  = rear_line;
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
void Steering::SetAngleManual(float front_angle, float rear_angle)
{
	//SetFrontAngle(front_angle);
	//SetRearAngle(rear_angle);

	front.angle = front_angle;
	rear.angle = rear_angle;
}

void Steering::Process()
{
    switch (mode)
    {
		case Off:
		{
			front.servo->Disable();
			rear.servo->Disable();
			break;
		}
    	case SingleLineFollow_Slow:
    	case SingleLineFollow_Fast:
    	case SingleLine_Race_Straight:
    	case SingleLine_Race_Decel:
    	case SingleLine_Race_Accel:
    	case SingleLine_Race_Turn:
        {
            front.controller->Process(front.line);

            SetFrontAngle(front.controller->GetControlValue());
            SetRearAngle(0.0f);
            break;
        }
    	case DualLineFollow_Slow:
    	case DualLine_Race_Straight:
    	{
            front.controller->Process(front.line);
            rear.controller->Process(rear.line);

            SetFrontAngle(front.controller->GetControlValue());
            SetRearAngle(rear.controller->GetControlValue());
    	    break;
    	}
    	case DualLineFollow_Fast:
    	{
    	    front.controller->Process(front.line);
    	    //rear.controller->Process(rear.line);

    	    SetFrontAngle(front.controller->GetControlValue());
    	    SetRearAngle(-front.controller->GetControlValue());
    	    break;
    	}
    	case DualLine_Race_Turn:
    	{
    	    front.controller->Process(front.line);

            SetFrontAngle(front.controller->GetControlValue());
            SetRearAngle(-front.controller->GetControlValue());
    	    break;
    	}
        case Free:
        {
        	SetFrontAngle(front.angle);
        	SetRearAngle(rear.angle);

            break;
        }
        default:
        	break;
    }
}

void Steering::SetFrontAngle(float angle /* rad */)
{
	float offset        = PI/2.0f + FRONT_OFFSET;
	float scale         = 1.0f;
	float servo_angle   = (angle + offset) * scale;

	front.servo->SetSteerAngle(servo_angle);
}

void Steering::SetRearAngle(float angle /* rad */)
{
	float offset        = PI/2.0f + REAR_OFFSET;
	float scale         = 1.0f;
	float servo_angle   = (angle + offset) * scale;

	rear.servo->SetSteerAngle(servo_angle);
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
