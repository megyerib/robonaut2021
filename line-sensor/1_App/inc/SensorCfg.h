#pragma once
#include "Stm32Gpio.h"

// SENSOR CONFIGURATION CHECKER
//
// Values can be set in the build configuration. This file only checks if the
// configuration is valid and generates a compilation time error if not.
// SENSOR_REV  : Hardware revision
//               1 : 2019
//               2 : 2020
// SENSOR_SIZE : Number of TCRT5000 sensors (24 or 32)

// REVISION DIFFERENCES ------------------------------------------------------------------------------------------------

// SPI rev1: --> | 15                          0 | 31                         16 | --+    Sensors
//               |  8         15 |  0          7 | 24         31 | 16         23 | <-+    LEDs
//
// SPI rev2: --> | 31                                                          0 |        Sensors
//           --> | 31                                                          0 |        LEDs

// ADCs measuring order is reversed also the order of the measuring set is reverse

#if (SENSOR_REV == 1)

#define MUX_E    PB2
#define TCRT_OE  PA8
#define LED_LE   PA12
#define LED_OE   PA15
#define TCRT_LE  PA11
#define MUX_S2   PA4
#define MUX_S1   PB0
#define MUX_S0   PB1

#else

#define MUX_E    PB0
#define TCRT_OE  PB1
#define LED_LE   PB2
#define LED_OE   PB10
#define TCRT_LE  PB11
#define MUX_S2   PB12
#define MUX_S1   PB13
#define MUX_S0   PB14

#endif

// CONFIGURATION VALIDITY CHECK ----------------------------------------------------------------------------------------

#if (SENSOR_REV == 1 && SENSOR_SIZE == 32)
	#define SENSOR_CONF_VALID
#endif

#if (SENSOR_REV == 2 && SENSOR_SIZE == 24)
	#define SENSOR_CONF_VALID
#endif

#if (SENSOR_REV == 2 && SENSOR_SIZE == 32)
	#define SENSOR_CONF_VALID
#endif

#ifndef SENSOR_CONF_VALID
#error "Sensor configuration invalid!"
#endif

// CONFIGURATION -------------------------------------------------------------------------------------------------------

#if (SENSOR_SIZE == 24)
#define GROUP_CNT           3
#endif

#if (SENSOR_SIZE == 32)
#define GROUP_CNT           4
#endif

#define IR_DIST_MM          9                               /* X distance of the IR sensors */
#define MID_IR_POS_MM       (IR_DIST_MM*(SENSOR_SIZE-1)/2)  /* Distance from the leftmost sensor to the middle of the sensor*/
#define GROUP_SIZE          8
