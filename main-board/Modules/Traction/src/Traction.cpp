#include "Traction.h"
#include <cstring>
#include "Encoder.h"
#include "MotorUart.h"

#define SATURATE(x, min, max)  ((x) = (x) > (max) ? (max) : ((x) < (min) ? (min) : (x)))

Traction::Traction()
{
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
	const size_t BUF_SIZE = 15; // 1 sign + 6 digits + 2 \r\n + safety
	static uint8_t buf[BUF_SIZE]; // Static because it needs to be sent through UART
	size_t bufSize;

	int message = d * 100000;

	SATURATE(message, -100000, 100000);

	PrintInt(buf, message, bufSize);

	MotorUart::GetInstance().Transmit(buf, SATURATE(bufSize, 0, BUF_SIZE) );
}

void Traction::PrintInt(void* dst, int n, size_t& size)
{
	const size_t BUF_SIZE = 15;
	uint8_t buf[BUF_SIZE];
	bool negative = false;

	buf[BUF_SIZE - 2] = '\r';
	buf[BUF_SIZE - 1] = '\n';

	size_t firstChar = 0;

	if (n < 0)
	{
		negative = true;
		n *= -1;
	}

	for (size_t i = BUF_SIZE - 3; i >= 0; i--)
	{
		buf[i] = '0' + (n % 10);
		n /= 10;

		if (n == 0)
		{
			firstChar = i;
			break;
		}
	}

	if (negative)
	{
		firstChar--;
		buf[firstChar] = '-';
	}

	size = BUF_SIZE - firstChar;
	memcpy(dst, &buf[firstChar], size);
}

void Traction::Process()
{
    // Speed control iteration
	controller->Process(Encoder::GetInstance().GetSpeed());

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
