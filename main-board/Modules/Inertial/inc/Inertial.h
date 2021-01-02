#pragma once
#include "stm32f4xx_hal.h"

// LSM6DSO inertial sensor
// https://www.st.com/en/mems-and-sensors/lsm6dso.html
// https://www.st.com/resource/en/application_note/dm00517282-lsm6dso-alwayson-3d-accelerometer-and-3d-gyroscope-stmicroelectronics.pdf

typedef struct
{
	bool     read; // read/!write
	uint8_t  addr; // register address
	uint8_t  data; // data buffer (both R/W -> needs to be static)
	bool     done; // is operation complete
}
I2C_OP; // I2C operation

class Inertial
{
public:
	static Inertial& GetInstance();

	void ConfigureSensor();
	void Process();

	void I2CEventIRQHandler();
	void I2CErrorIRQHandler();
	void Int1Handler();
	void Int2Handler();

private:
	Inertial();

	HAL_StatusTypeDef WriteReg(uint8_t addr, uint8_t  val);
	HAL_StatusTypeDef ReadReg (uint8_t addr, uint8_t* buf);

	void InitI2C();
	void InitI2CGPIO();
	void InitEXTIGPIO();

	I2C_HandleTypeDef hi2c;
};
