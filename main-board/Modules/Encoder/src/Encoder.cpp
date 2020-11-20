#include "Encoder.h"
#include "Trace.h"
#include "SerialMessages.h"

#define INC_PER_M    (140331.0f)
// The whole 32 bit counter range is more than 30 km

Encoder::Encoder()
{
	enc      = EncoderHw::GetInstance();
	usTimer  = Uptime::GetInstance();
}

Encoder* Encoder::GetInstance()
{
	static Encoder instance;
	return &instance;
}

float Encoder::GetDistance()
{
	return mPoints[mPointIndex].encVal / INC_PER_M;
}

// The calculation won't be valid in the first 'ENCODER_MEASURE_POINTS' cycles but who cares?
float Encoder::GetSpeed()
{
	__disable_irq();
	int32_t  enc_prev = mPoints[(mPointIndex + 1)%ENCODER_MEASURE_POINTS].encVal;
	int32_t  enc_cur  = mPoints[mPointIndex].encVal;
	uint32_t tim_prev = mPoints[(mPointIndex + 1)%ENCODER_MEASURE_POINTS].timVal;
	uint32_t tim_cur  = mPoints[mPointIndex].timVal;
	__enable_irq();

	float d_s = (enc_cur - enc_prev) / INC_PER_M;  // m
	float d_t = (tim_cur - tim_prev) / 1000000.0;  // s

	if (d_t == 0)
	{
		return 0; // Div by 0
	}
	else
	{
		return d_s/d_t;
	}
}

void Encoder::Process()
{
	__disable_irq();
	mPointIndex++;
	mPointIndex %= ENCODER_MEASURE_POINTS;

	mPoints[mPointIndex].encVal = (int32_t) enc->GetCounterValue();
	mPoints[mPointIndex].timVal = usTimer->GetTime();
	__enable_irq();

	// Trace
	/*SM_DUMMY msg;
	msg.id = smDummy;
	msg.timestamp = UPTIME();
	msg.value = (GetSpeed() * 1000); // TODO make it more efficient

	TRACE_BIN(&msg, sizeof(msg));*/
}
