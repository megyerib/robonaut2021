#pragma once

#include "Evaluator.h"

class StddevEval : public Evaluator
{
	AdcMeasType data[SENSOR_SIZE];

public:
	StddevEval();

	void Feed(AdcMeasType meas[SENSOR_SIZE]) override;
	LineInput GetLine() override;

private:
	uint32_t max4_pos(int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
	    void magicDiff(uint32_t* src, uint32_t* dst);
	 int16_t ledPosToMm(uint8_t ledPos);
	 int32_t evalWeightedMean(uint32_t* arr, uint32_t i);
	uint32_t evalIsPeak(uint32_t* arr, uint32_t i, uint32_t mean, uint32_t stdDev);

	 // Math
	 uint32_t mean(uint32_t* data, uint32_t num);
	 uint32_t standardDeviation(uint32_t* data, uint32_t num, uint32_t avg);
};
