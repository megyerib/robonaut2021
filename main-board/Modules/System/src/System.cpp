#include "System.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_pwr_ex.h"
#include "FreeRTOS.h"
#include "task.h"

System::System()
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	Clock_Config();
	HAL_InitTick(TICK_INT_PRIORITY);
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
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	// Configure the main internal regulator output voltage
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	// Initialize the CPU, AHB and APB busses clocks
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 180;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;

	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	// Initialize the CPU, AHB and APB busses clocks
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
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
