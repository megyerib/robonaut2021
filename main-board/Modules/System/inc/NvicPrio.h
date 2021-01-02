#pragma once
#include "FreeRTOSConfig.h"

// lower value -> higher priority

// see HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority)

// The RTOS interrupt nesting scheme splits the available interrupt priorities into two groups – those that will get
// masked by RTOS critical sections, and those that are never masked by RTOS critical sections and are therefore always
// enabled. The configMAX_SYSCALL_INTERRUPT_PRIORITY setting in FreeRTOSConfig.h defines the boundary between the two
// groups. The optimal value for this setting will depend on the number of priority bits implemented in the
// microcontroller.
// https://www.freertos.org/RTOS-Cortex-M3-M4.html

#define TRACE_UART_NVIC_PRIO       2
#define DMA_UART_NVIC_PRIO         2  /* Misc DMA UART peripherals */
#define DMA_NVIC_PRIO              2
#define TOF_I2C_NVIC_PRIO          5
#define IMU_I2C_NVIC_PRIO          6
#define IMU_EXTI_NVIC_PRIO         4
