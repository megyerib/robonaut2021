// TIM3 HW Channel 4 <-> Remote channel 3

#include "../inc/RemoteTimer.h"

#define VALID_LOW    500
#define VALID_HIGH  2500

uint16_t RemoteHw::data[RemChNum][DATA_BUF_SIZE];

RemoteHw::RemoteHw()
{
	InitGpio();
	InitTimer();
	InitDma();

	HAL_TIM_IC_Start_DMA(&htim, TIM_CHANNEL_1, (uint32_t*) data[RemCh1], DATA_BUF_SIZE);
	htim.State = HAL_TIM_STATE_READY; // HAL bug
	HAL_TIM_IC_Start_DMA(&htim, TIM_CHANNEL_2, (uint32_t*) data[RemCh2], DATA_BUF_SIZE);
	htim.State = HAL_TIM_STATE_READY; // HAL bug
	HAL_TIM_IC_Start_DMA(&htim, TIM_CHANNEL_4, (uint32_t*) data[RemCh3], DATA_BUF_SIZE);
	htim.State = HAL_TIM_STATE_READY; // HAL bug
}

RemoteHw* RemoteHw::GetInstance()
{
	static RemoteHw instance;
	return &instance;
}

uint16_t RemoteHw::GetPulseWidth(RemoteHwChannel ch)
{
	uint32_t index;
	uint16_t edge[3];
	uint16_t pulse[2];

	__disable_irq(); // This part shall be performed fast to avoid DMA overrun

	index = DATA_BUF_SIZE - hdma[ch].Instance->NDTR;

	edge[0] = data[ch][(index - 3) % DATA_BUF_SIZE];
	edge[1] = data[ch][(index - 2) % DATA_BUF_SIZE];
	edge[2] = data[ch][(index - 1) % DATA_BUF_SIZE];

	__enable_irq();

	pulse[0] = edge[1] - edge[0];
	pulse[1] = edge[2] - edge[1];

	uint16_t ret = (pulse[0] < pulse[1]) ? pulse[0] : pulse[1];

	if (ret > 2500) // Invalid value
	{
		ret = prev[ch];
	}
	else
	{
		prev[ch] = ret;
	}

	return ret;
}

void RemoteHw::InitGpio()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/**TIM3 GPIO Configuration
	PA6     ------> TIM3_CH1
	PA7     ------> TIM3_CH2
	PB1     ------> TIM3_CH4
	*/

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void RemoteHw::InitTimer()
{
	__HAL_RCC_TIM3_CLK_ENABLE();

	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};

	htim.Instance                    = TIM3;
	htim.Init.Prescaler              = 90 - 1;
	htim.Init.CounterMode            = TIM_COUNTERMODE_UP;
	htim.Init.Period                 = 0xFFFF;
	htim.Init.ClockDivision          = TIM_CLOCKDIVISION_DIV1;
	htim.Init.AutoReloadPreload      = TIM_AUTORELOAD_PRELOAD_DISABLE;

	HAL_TIM_IC_Init(&htim);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;

	HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

	sConfigIC.ICPolarity              = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
	sConfigIC.ICSelection             = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler             = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter                = 0;

	HAL_TIM_IC_ConfigChannel(&htim, &sConfigIC, TIM_CHANNEL_1);
	HAL_TIM_IC_ConfigChannel(&htim, &sConfigIC, TIM_CHANNEL_2);
	HAL_TIM_IC_ConfigChannel(&htim, &sConfigIC, TIM_CHANNEL_4);
}

void RemoteHw::InitDma()
{
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* TIM3 DMA Init */
	/* TIM3_CH1_TRIG Init */
	hdma[RemCh1].Instance = DMA1_Stream4;
	hdma[RemCh1].Init.Channel = DMA_CHANNEL_5;
	hdma[RemCh1].Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma[RemCh1].Init.PeriphInc = DMA_PINC_DISABLE;
	hdma[RemCh1].Init.MemInc = DMA_MINC_ENABLE;
	hdma[RemCh1].Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma[RemCh1].Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma[RemCh1].Init.Mode = DMA_CIRCULAR;
	hdma[RemCh1].Init.Priority = DMA_PRIORITY_LOW;
	hdma[RemCh1].Init.FIFOMode = DMA_FIFOMODE_DISABLE;

	HAL_DMA_Init(&hdma[RemCh1]);
	__HAL_LINKDMA(&htim,hdma[TIM_DMA_ID_CC1],hdma[RemCh1]);

	HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

	/* TIM3_CH2 Init */
	hdma[RemCh2].Instance = DMA1_Stream5;
	hdma[RemCh2].Init.Channel = DMA_CHANNEL_5;
	hdma[RemCh2].Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma[RemCh2].Init.PeriphInc = DMA_PINC_DISABLE;
	hdma[RemCh2].Init.MemInc = DMA_MINC_ENABLE;
	hdma[RemCh2].Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma[RemCh2].Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma[RemCh2].Init.Mode = DMA_CIRCULAR;
	hdma[RemCh2].Init.Priority = DMA_PRIORITY_LOW;
	hdma[RemCh2].Init.FIFOMode = DMA_FIFOMODE_DISABLE;

	HAL_DMA_Init(&hdma[RemCh2]);
	__HAL_LINKDMA(&htim,hdma[TIM_DMA_ID_CC2],hdma[RemCh2]);

	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

	/* TIM3_CH4_UP Init */
	hdma[RemCh3].Instance = DMA1_Stream2;
	hdma[RemCh3].Init.Channel = DMA_CHANNEL_5;
	hdma[RemCh3].Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma[RemCh3].Init.PeriphInc = DMA_PINC_DISABLE;
	hdma[RemCh3].Init.MemInc = DMA_MINC_ENABLE;
	hdma[RemCh3].Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma[RemCh3].Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma[RemCh3].Init.Mode = DMA_CIRCULAR;
	hdma[RemCh3].Init.Priority = DMA_PRIORITY_LOW;
	hdma[RemCh3].Init.FIFOMode = DMA_FIFOMODE_DISABLE;

	HAL_DMA_Init(&hdma[RemCh3]);
	__HAL_LINKDMA(&htim,hdma[TIM_DMA_ID_CC4],hdma[RemCh3]);

	HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
}

void RemoteHw::Ch1DmaIrq()
{
	HAL_DMA_IRQHandler(&hdma[RemCh1]);
}

void RemoteHw::Ch2DmaIrq()
{
	HAL_DMA_IRQHandler(&hdma[RemCh2]);
}

void RemoteHw::Ch4DmaIrq()
{
	HAL_DMA_IRQHandler(&hdma[RemCh3]);
}

extern "C" void DMA1_Stream2_IRQHandler(void)
{
	RemoteHw::GetInstance()->Ch4DmaIrq();
}

extern "C" void DMA1_Stream4_IRQHandler(void)
{
	RemoteHw::GetInstance()->Ch1DmaIrq();
}

extern "C" void DMA1_Stream5_IRQHandler(void)
{
	RemoteHw::GetInstance()->Ch2DmaIrq();
}
