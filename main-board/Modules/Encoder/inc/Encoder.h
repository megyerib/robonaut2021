#pragma once

#include "EncoderTimer.h"

#define ENCODER_SAMPLING_PERIOD   10 /* ms */
// The accurate timing values will be read from the us timer.

#define ENCODER_MEASURE_POINTS     4

typedef struct
{
	int32_t  encVal;
	uint32_t timVal;
}
EncMeasPoint;

class Encoder
{
public:
	static Encoder* GetInstance();
	float GetDistance(); /* m */
	float GetSpeed(); /* m/s */
	void Process();

private:
	EncoderHw* enc;

	EncMeasPoint mPoints[ENCODER_MEASURE_POINTS];
	size_t mPointIndex = 0;

	Encoder();
};
