#pragma once
#include "FreeRTOSConfig.h"

// HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority)
// lower value -> higher priority

// The RTOS interrupt nesting scheme splits the available interrupt priorities into two groups – those that will get
// masked by RTOS critical sections, and those that are never masked by RTOS critical sections and are therefore always
// enabled. The configMAX_SYSCALL_INTERRUPT_PRIORITY setting in FreeRTOSConfig.h defines the boundary between the two
// groups. The optimal value for this setting will depend on the number of priority bits implemented in the
// microcontroller.
// https://www.freertos.org/RTOS-Cortex-M3-M4.html

#define TRACE_UART_NVIC_PRIO           configMAX_SYSCALL_INTERRUPT_PRIORITY - 3
#define DMA_NVIC_PRIO              2
