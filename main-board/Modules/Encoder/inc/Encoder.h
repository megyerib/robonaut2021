#pragma once

#include <EncoderHw.h>

#define ENC_SAMPLING_PERIOD_ms    (5)
// The accurate timing values will be read from the us timer.

#define ENC_AVGING_WINDOW         (4)
#define ENC_SAMPLES               (ENC_AVGING_WINDOW + 1)

typedef struct
{
	int32_t  encVal;
	uint32_t timVal;
}
EncMeasPoint;

class Encoder
{
public:
	static Encoder& GetInstance();
	float GetDistance(); /* m */
	float GetSpeed(); /* m/s */
	void Process();

private:
	float speeds[5] = {0}; // TODO macro
	size_t iSpeed = 0;
	float distance = 0;

	EncoderHw& enc;

	EncMeasPoint mPoints[ENC_SAMPLES];
	size_t iMP = 0;

	Encoder();
};
