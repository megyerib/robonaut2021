#include "Traction.h"
#include <cstring>
#include "Encoder.h"
#include "MotorUart.h"

#define SATURATE(x, min, max)  ((x) = (x) > (max) ? (max) : ((x) < (min) ? (min) : (x)))

Traction::Traction()
{
	dutyCycle = 0;
	mode = tmDutyCycle;
}

Traction* Traction::GetInstance()
{
	static Traction instance;
	return &instance;
}

void Traction::SendDutyCycle(float d)
{
	const size_t BUF_SIZE = 15; // 1 sign + 6 digits + 2 \r\n + safety
	static char buf[BUF_SIZE]; // Static because it needs to be sent through UART
	size_t bufSize;

	int message = d * 100000;

	SATURATE(message, -100000, 100000);

	bufSize = sprintint(buf, message);

	buf[bufSize++] = '\r';
	buf[bufSize++] = '\n';

	MotorUart::GetInstance().Transmit(buf, bufSize);
}

int Traction::sprintint(char* dst, int n)
{
	const size_t BUF_SIZE = 15;
	uint8_t buf[BUF_SIZE];
	bool negative = false;

	size_t firstChar = BUF_SIZE;

	if (n < 0)
	{
		negative = true;
		n *= -1;
	}

	for (size_t i = BUF_SIZE - 1; i >= 0; i--)
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

	size_t size = BUF_SIZE - firstChar;
	memcpy(dst, &buf[firstChar], size);
	dst[size] = '\0'; // Just in case

	return size;
}

void Traction::Process()
{
    switch (mode)
    {
		case tmDutyCycle:
		{
			SendDutyCycle(dutyCycle);
			break;
		}
		default:
		{
			break;
		}
    }
}
