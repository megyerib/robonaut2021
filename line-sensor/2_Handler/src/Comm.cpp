#include "Comm.h"
extern "C"
{
#include "base64.h"
}

#define OUTPUT_DATA_SIZE 30

Comm::Comm()
{
	uart = Uart::GetInstance();
}

Comm* Comm::GetInstance()
{
	static Comm instance;

	return &instance;
}

void Comm::SendLine(LineInput* l)
{
	uint8_t buf[OUTPUT_DATA_SIZE];
	size_t buflen = 0;

	base64_encode((uint8_t*)l, buf, sizeof(LineInput), &buflen);

	buf[buflen] = '\n';
	buflen++;

	uart->Send(buf, buflen);
}
