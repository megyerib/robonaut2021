#include "Encoder.h"
#include "Trace.h"
#include "SerialMessages.h"
#include "Uptime.h"

#define INC_PER_M    (140331.0f)
// The whole 32 bit counter range is more than 30 km

Encoder::Encoder() : enc(EncoderHw::GetInstance())
{

}

Encoder& Encoder::GetInstance()
{
	static Encoder instance;
	return instance;
}

float Encoder::GetDistance()
{
	return distance;
}

// The calculation won't be valid in the first 'ENC_SAMPLES' cycles
float Encoder::GetSpeed()
{
	return speed;
}

void Encoder::Process()
{
	iMP++;
	iMP %= ENC_SAMPLES;

	mPoints[iMP].encVal = enc.GetCounterValue();
	mPoints[iMP].timVal = UPTIME_us();

	// Distance
	distance = mPoints[iMP].encVal / INC_PER_M;

	// Speed
	size_t i1st = (iMP + 1) % ENC_SAMPLES;

	float d_s = (mPoints[iMP].encVal - mPoints[i1st].encVal) / INC_PER_M;  // m
	float d_t = (mPoints[iMP].timVal - mPoints[i1st].timVal) / 1000000.0;  // s

	speed = (d_t != 0) ? (d_s / d_t) : 0;

	// Trace (dummy)
	/*SM_DUMMY msg;
	msg.id        = smDummy;
	msg.timestamp = UPTIME_us();
	msg.value     = speed * 1000;
	TRACE_BIN(&msg, sizeof(msg));*/
}
