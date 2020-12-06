#include "Uptime.h"

// There is no 32 bit timer on the F0
// Two 16 bit timers shall be connected
// Master / low  : TIM1
// Slave  / high : TIM3

Uptime::Uptime()
{
	InitMasterTimer();
	InitSlaveTimer();

	// Start timer
	HAL_TIM_Base_Start(&htim_high); // Slave
	HAL_TIM_Base_Start(&htim_low);  // Master
}

Uptime& Uptime::GetInstance()
{
	static Uptime instance;
	return instance;
}

uint32_t Uptime::GetTime()
{
	// Read 2x16 bit timer safely:
	// https://www.rapitasystems.com/blog/chaining-two-16-bit-timers-together-stm32f4

	uint32_t lsw_1 = __HAL_TIM_GET_COUNTER(&htim_low);
	uint32_t msw   = __HAL_TIM_GET_COUNTER(&htim_high);
	uint32_t lsw_2 = __HAL_TIM_GET_COUNTER(&htim_low);

	__disable_irq();
	lsw_1 = __HAL_TIM_GET_COUNTER(&htim_low);
	msw   = __HAL_TIM_GET_COUNTER(&htim_high);
	lsw_2 = __HAL_TIM_GET_COUNTER(&htim_low);

	if (lsw_2 < lsw_1) // has master/low timer rolled over between its first and second reading?
	{
		// rollover has happened. lsw_2 is read post-rollover.
		// Not sure whether msw was read pre- or post-rollover
		msw = __HAL_TIM_GET_COUNTER(&htim_high);
	}
	__enable_irq();

	return lsw_2 + (msw << 16);
}

void Uptime::InitMasterTimer()
{
  TIM_ClockConfigTypeDef  sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig      = {0};

  __HAL_RCC_TIM1_CLK_ENABLE();

  htim_low.Instance                    = TIM1;
  htim_low.Init.Prescaler              = 0;
  htim_low.Init.CounterMode            = TIM_COUNTERMODE_UP;
  htim_low.Init.Period                 = 0xFFFF;
  htim_low.Init.ClockDivision          = TIM_CLOCKDIVISION_DIV1;
  htim_low.Init.RepetitionCounter      = 0;
  htim_low.Init.AutoReloadPreload      = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim_low);

  sClockSourceConfig.ClockSource    = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim_low, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_ENABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim_low, &sMasterConfig);
}

void Uptime::InitSlaveTimer()
{
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};

  __HAL_RCC_TIM3_CLK_ENABLE();

  htim_high.Instance                    = TIM3;
  htim_high.Init.Prescaler              = 48 - 1;
  htim_high.Init.CounterMode            = TIM_COUNTERMODE_UP;
  htim_high.Init.Period                 = 0xFFFF;
  htim_high.Init.ClockDivision          = TIM_CLOCKDIVISION_DIV1;
  htim_high.Init.AutoReloadPreload      = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim_high);

  sSlaveConfig.SlaveMode            = TIM_SLAVEMODE_EXTERNAL1;
  sSlaveConfig.InputTrigger         = TIM_TS_ITR0;
  HAL_TIM_SlaveConfigSynchro(&htim_high, &sSlaveConfig);
}
