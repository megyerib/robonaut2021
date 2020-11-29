#include "SensorMeasure.h"
#include "stm32f0xx_hal.h"
#include <string.h>
#include "Stm32Gpio.h"
#include "SensorCfg.h"

ADC_HandleTypeDef SensorMeasure::handle;

SensorMeasure::SensorMeasure()
{
	InitAdcGpio();
	InitAdc();
	InitMux();
}

SensorMeasure* SensorMeasure::GetInstance()
{
	static SensorMeasure instance;

	return &instance;
}

void SensorMeasure::Measure(AdcInput input)
{
	SetMux(input);

	HAL_ADC_Start(&handle);

	for (int i = 0; i <= 4; i++)
	{
		HAL_ADC_PollForConversion(&handle, 1 /* ms timeout */);
		AdcMeasType meas = HAL_ADC_GetValue(&handle);

		measurements[31-(i*8+input)] = meas;
	}

	HAL_ADC_Stop(&handle);
}

void SensorMeasure::GetMeasurements(AdcMeasType* dest)
{
#if (SENSOR_REV == 1)
	for (int i = 0; i < SENSOR_SIZE; i++)
	{
		dest[i] = measurements[SENSOR_SIZE - 1 - i];
	}
#elif (SENSOR_REV == 2)
	size_t size  = SENSOR_SIZE * sizeof(AdcMeasType);
	memcpy(dest, measurements, size);
#endif
}

void SensorMeasure::InitMux()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* Configure GPIO pins */
	GPIO_InitStruct.Pin   = 1 << Stm32Gpio::GetPin(MUX_S0);
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(Stm32Gpio::GetPort(MUX_S0), &GPIO_InitStruct);

	GPIO_InitStruct.Pin   = 1 << Stm32Gpio::GetPin(MUX_S1);
	HAL_GPIO_Init(Stm32Gpio::GetPort(MUX_S1), &GPIO_InitStruct);

	GPIO_InitStruct.Pin   = 1 << Stm32Gpio::GetPin(MUX_S2);
	HAL_GPIO_Init(Stm32Gpio::GetPort(MUX_S2), &GPIO_InitStruct);

	GPIO_InitStruct.Pin   = 1 << Stm32Gpio::GetPin(MUX_E);
	HAL_GPIO_Init(Stm32Gpio::GetPort(MUX_E), &GPIO_InitStruct);
}

void SensorMeasure::InitAdcGpio()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/**ADC GPIO Configuration
    PA0     ------> ADC_IN0
	PA1     ------> ADC_IN1
	PA2     ------> ADC_IN2
	PA3     ------> ADC_IN3
	*/

	GPIO_InitStruct.Pin  = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void SensorMeasure::InitAdc()
{
	/* ADC1 clock enable */
	__HAL_RCC_ADC1_CLK_ENABLE();

	ADC_ChannelConfTypeDef sConfig;

	/**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	*/
	handle.Instance                   = ADC1;
	handle.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;
	handle.Init.Resolution            = ADC_RESOLUTION_12B;
	handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;

#if (SENSOR_REV == 1)
	handle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_BACKWARD;
#elif (SENSOR_REV == 2)
	handle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;
#endif

	handle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
	handle.Init.LowPowerAutoWait      = DISABLE;
	handle.Init.LowPowerAutoPowerOff  = DISABLE;
	handle.Init.ContinuousConvMode    = ENABLE;
	handle.Init.DiscontinuousConvMode = DISABLE;
	handle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
	handle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
	handle.Init.DMAContinuousRequests = DISABLE;
	handle.Init.Overrun               = ADC_OVR_DATA_PRESERVED;

	HAL_ADC_Init(&handle);

	sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	// ADC_SAMPLETIME_41CYCLES_5 is the smallest ok value

	const uint32_t AdcChannels[4] =
	{
		ADC_CHANNEL_0,
		ADC_CHANNEL_1,
		ADC_CHANNEL_2,
		ADC_CHANNEL_3
	};

	for (int i = 0; i < 4; i++)
	{
		sConfig.Channel = AdcChannels[i];
		HAL_ADC_ConfigChannel(&handle, &sConfig);
	}
}

void SensorMeasure::SetMux(AdcInput input)
{
	// Propagation delay < 60 ns
	// T_clk ~ 20 ns; a function call lasts longer than that

	HAL_GPIO_WritePin(Stm32Gpio::GetPort(MUX_S2), 1 << Stm32Gpio::GetPin(MUX_S2), GPIO_PinState((input >> 2) & 1));
	HAL_GPIO_WritePin(Stm32Gpio::GetPort(MUX_S1), 1 << Stm32Gpio::GetPin(MUX_S1), GPIO_PinState((input >> 1) & 1));
	HAL_GPIO_WritePin(Stm32Gpio::GetPort(MUX_S0), 1 << Stm32Gpio::GetPin(MUX_S0), GPIO_PinState((input >> 0) & 1));
}
