#pragma once
#include "stm32f0xx_hal.h"
#include <stdint.h>

#ifndef HAL_ADC_MODULE_ENABLED
	#error Define HAL_ADC_MODULE_ENABLED in stm32xxxx_hal_conf.h
#endif

typedef enum
{
	Input0,
	Input1,
	Input2,
	Input3,
	Input4,
	Input5,
	Input6,
	Input7
}
AdcInput;

typedef uint32_t AdcMeasType;

class SensorMeasure
{
public:
	static SensorMeasure* GetInstance();
	void Measure(AdcInput input);
	void GetMeasurements(AdcMeasType* dest);

private:
	static ADC_HandleTypeDef handle;
	AdcMeasType measurements[32];

	SensorMeasure();

	void InitMux();
	void InitAdc();
	void InitAdcGpio();

	void SetMux(AdcInput input);
};
