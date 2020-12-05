#pragma once

#include "SensorMeasure.h"
#include "Line.h"

class Evaluator
{
public:
	// Push an iteration of measurment data to the evaluator
	virtual void Feed(AdcMeasType meas[SENSOR_SIZE]) = 0;

	// Get the actual evaluated line
	virtual LineInput GetLine() = 0;
};
