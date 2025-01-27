#pragma once

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

#define MUX_E_port    GPIOB
#define TCRT_OE_port  GPIOA
#define LED_LE_port   GPIOA
#define LED_OE_port   GPIOA
#define TCRT_LE_port  GPIOA
#define MUX_S2_port   GPIOA
#define MUX_S1_port   GPIOB
#define MUX_S0_port   GPIOB

#define MUX_E_pin     GPIO_PIN_2
#define TCRT_OE_pin   GPIO_PIN_8
#define LED_LE_pin    GPIO_PIN_12
#define LED_OE_pin    GPIO_PIN_15
#define TCRT_LE_pin   GPIO_PIN_11
#define MUX_S2_pin    GPIO_PIN_4
#define MUX_S1_pin    GPIO_PIN_0
#define MUX_S0_pin    GPIO_PIN_1

#else

#define MUX_E_port    GPIOB
#define IR_OE_port    GPIOB
#define LED_LE_port   GPIOB
#define LED_OE_port   GPIOB
#define IR_LE_port    GPIOB
#define MUX_S2_port   GPIOB
#define MUX_S1_port   GPIOB
#define MUX_S0_port   GPIOB

#define MUX_E_pin     GPIO_PIN_0
#define IR_OE_pin     GPIO_PIN_1
#define LED_LE_pin    GPIO_PIN_2
#define LED_OE_pin    GPIO_PIN_10
#define IR_LE_pin     GPIO_PIN_11
#define MUX_S2_pin    GPIO_PIN_12
#define MUX_S1_pin    GPIO_PIN_13
#define MUX_S0_pin    GPIO_PIN_14

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

#define IR_GROUP_SIZE    4   /* Every Nth IR LED is illuminated in a single iteration */

#define IR_DIST_UNIT        2108    /* X distance of the IR sensors (line2021\line-sensor-unit.xlsx) */
#define MID_IR_POS_UNIT     (IR_DIST_UNIT*(SENSOR_SIZE-1)/2)  /* Distance from the leftmost sensor to the middle of the sensor*/
#define GROUP_SIZE          8
