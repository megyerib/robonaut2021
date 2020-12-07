#include "System.h"

#include "stm32f0xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

System::System()
{
#if (PREFETCH_ENABLE != 0)
	__HAL_FLASH_PREFETCH_BUFFER_ENABLE();
#endif /* PREFETCH_ENABLE */

	//HAL_InitTick(TICK_INT_PRIORITY);

	Clock_Config(); // HAL_InitTick is also called here

	// System interrupt init
	// SVC_IRQn interrupt configuration
	HAL_NVIC_SetPriority(SVC_IRQn, 0, 0);
	// PendSV_IRQn interrupt configuration
	HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
	// SysTick_IRQn interrupt configuration
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

System& System::GetInstance()
{
	static System instance;
	return instance;
}

void System::OsStart()
{
	vTaskStartScheduler();
}

void System::Clock_Config()
{
	// Bus & timer frequencies: 48 MHz

#if !HSE_OSCILLATOR
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();

	RCC_OscInitTypeDef RCC_OscInitStruct    = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct    = {0};
	//RCC_PeriphCLKInitTypeDef PeriphClkInit     = {0};

	/** Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_OscInitStruct.OscillatorType        = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
	RCC_OscInitStruct.HSIState              = RCC_HSI_ON;
	RCC_OscInitStruct.HSI14State            = RCC_HSI14_ON;
	RCC_OscInitStruct.HSICalibrationValue   = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.HSI14CalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState          = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource         = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL            = RCC_PLL_MUL12;
	RCC_OscInitStruct.PLL.PREDIV            = RCC_PREDIV_DIV1;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	/** Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_ClkInitStruct.ClockType             = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							                  |RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource          = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider         = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider        = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

	//PeriphClkInit.PeriphClockSelection      = RCC_PERIPHCLK_USART1;
	//PeriphClkInit.Usart1ClockSelection      = RCC_USART1CLKSOURCE_PCLK1;
	//HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
#else
	RCC_OscInitTypeDef RCC_OscInitStruct    = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct    = {0};

	// Enable oscillator GPIO
	__HAL_RCC_GPIOF_CLK_ENABLE();

	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType        = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState              = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState          = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource         = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL            = RCC_PLL_MUL6;
	RCC_OscInitStruct.PLL.PREDIV            = RCC_PREDIV_DIV1;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType             = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							                  |RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource          = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider         = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider        = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
#endif
}

// Systick handler
extern "C" void SysTick_Handler(void)
{
	// HAL
	HAL_IncTick();

	// FreeRTOS
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
	{
		xPortSysTickHandler();
	}
}
