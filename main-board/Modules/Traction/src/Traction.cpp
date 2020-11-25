#include "Traction.h"
#include "printf.h" /* Lightweight printf */

Traction::Traction()
{
	uart = MotorUart::GetInstance();
	encoder = Encoder::GetInstance();
	controller = new Pid_Controller(0.002f, 0.008f, 0.0f);
	controller->Set_I_Limit(20.0f);

	targetDutyCycle = 0;
	prevDutyCycle   = 0;
	forceDutyCycle  = 0;

	mode = tmode_Manual;
}

Traction* Traction::GetInstance()
{
	static Traction instance;
	return &instance;
}

void Traction::SetSpeed(float speed /* m/s */)
{
    controller->SetSetpoint(speed);
}

void Traction::SetMode(TractionMode mode)
{
	this->mode = mode;
}

void Traction::SetControllerIntegrationLimit(float const limit)
{
    controller->Set_I_Limit(limit);
}

void Traction::SetDutyCycle(float d /* % [-1;+1] */)
{
    controller->Reset();
    forceDutyCycle = d;
}

void Traction::SendDutyCycle(float d /* % [-1;+1] */)
{
	static uint8_t buf[10];

	int message = d * 100000;
	int size;

	// TODO lightweight print & remove printf.h
	size = sprintf((char*)buf, "%d\r\n", message);

	uart->Transmit(buf, size);
}

void Traction::Process()
{
    // Speed control iteration
	controller->Process(encoder->GetSpeed());

	// Control value ramping
	if ((controller->GetControlValue() - prevDutyCycle) > 0.01f)
	{
	    targetDutyCycle = prevDutyCycle + 0.01f;
	}
	else if ((controller->GetControlValue() - prevDutyCycle) < -0.06f)
	{
	    targetDutyCycle = prevDutyCycle - 0.06f;
	}
	else
	{
	    targetDutyCycle = controller->GetControlValue();
	}

	// Saturation
    if (targetDutyCycle > 0.67f)
    {
        targetDutyCycle = 0.67f;
    }
    else if (targetDutyCycle < -0.3f)
    {
        targetDutyCycle = -0.3f;
    }

     // Deadband
    if ((controller->Get_Setpoint() < 0.05f) && (controller->Get_Setpoint() > -0.05f))
    {
        targetDutyCycle = 0;
    }


    // Actuate
    if (mode == tmode_Controller)
    {
        SendDutyCycle(targetDutyCycle);
    }
    else
    {
        SendDutyCycle(forceDutyCycle);
    }

	prevDutyCycle = targetDutyCycle;
}
