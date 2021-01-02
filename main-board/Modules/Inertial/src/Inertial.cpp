#include "Inertial.h"
#include "lsm6dso_reg.h"
#include "Uptime.h"
#include "Trace.h"
#include "NvicPrio.h"

const uint8_t addresses[12] =
{
	LSM6DSO_OUTX_L_A,
	LSM6DSO_OUTX_H_A,
	LSM6DSO_OUTY_L_A,
	LSM6DSO_OUTY_H_A,
	LSM6DSO_OUTZ_L_A,
	LSM6DSO_OUTZ_H_A,

	LSM6DSO_OUTX_L_G,
	LSM6DSO_OUTX_H_G,
	LSM6DSO_OUTY_L_G,
	LSM6DSO_OUTY_H_G,
	LSM6DSO_OUTZ_L_G,
	LSM6DSO_OUTZ_H_G
};

Inertial::Inertial()
{
	InitI2CGPIO(); // Needs to be called before InitI2C; Otherwise I2C will stuck in HAL_BUSY state
	InitI2C();
	InitEXTIGPIO();

	ConfigureSensor();
}

Inertial& Inertial::GetInstance()
{
	static Inertial instance;
	return instance;
}

void Inertial::Process()
{


	int16_t vals[6];

	uint8_t* rxBuf = (uint8_t*)vals;

	for (int i = 0; i < 12; i++)
	{
		ReadReg(addresses[i], rxBuf);
		rxBuf++;
	}

	return;
}

void Inertial::ConfigureSensor()
{
	// See 4.1 Startup sequence
	uint8_t whoAmI;
	HAL_StatusTypeDef status = ReadReg(LSM6DSO_WHO_AM_I, &whoAmI);

	if (status != HAL_OK)
	{
		PRINTF("Inertial sensor i2c fail (%d).", status);
	}
	else if (whoAmI != 0x6C)
	{
		PRINTF("Inertial sensor incorrect WHO_AM_I value (%d).", whoAmI);
	}
	else
	{
		PRINTF("Inertial sensor identified successfully.");
	}

	// 1. Write INT1_CTRL = 01h // Acc data-ready interrupt on INT1
	// Nope

	// 2. Write CTRL1_XL  = 60h // Acc = 417 Hz (High-Performance mode)
	WriteReg(LSM6DSO_CTRL1_XL, 0x60);

	// 1. Write INT1_CTRL = 02h // Gyro data-ready interrupt on INT1
	// 2. Write CTRL2_G   = 60h // Gyro = 417 Hz (High-Performance mode)
	WriteReg(LSM6DSO_CTRL2_G, 0x60);

	// TODO CTRL7_G High pass filter
}

void Inertial::InitI2C()
{
	__HAL_RCC_I2C3_CLK_ENABLE();

	// Reset I2C peripheral
	SET_BIT(RCC->APB1RSTR, RCC_APB1RSTR_I2C3RST);
	CLEAR_BIT(RCC->APB1RSTR, RCC_APB1RSTR_I2C3RST);

	hi2c.Instance              = I2C3;
	hi2c.Init.ClockSpeed       = 100000;
	hi2c.Init.DutyCycle        = I2C_DUTYCYCLE_2;
	hi2c.Init.OwnAddress1      = 0;
	hi2c.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
	hi2c.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
	hi2c.Init.OwnAddress2      = 0;
	hi2c.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
	hi2c.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

	HAL_I2C_Init(&hi2c);

	// Interrupts (they must have the same prio not to interrupt eachother)
	HAL_NVIC_SetPriority(I2C3_EV_IRQn, IMU_I2C_NVIC_PRIO, 0);
	HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
	HAL_NVIC_SetPriority(I2C3_ER_IRQn, IMU_I2C_NVIC_PRIO, 0);
	HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);
}

void Inertial::InitI2CGPIO()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// I2C3 GPIO Configuration:
	//  PC9     ------> I2C3_SDA
	GPIO_InitStruct.Pin        = GPIO_PIN_9;
	GPIO_InitStruct.Mode       = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull       = GPIO_NOPULL;
	GPIO_InitStruct.Speed      = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate  = GPIO_AF4_I2C3;

	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	//  PA8     ------> I2C3_SCL
	GPIO_InitStruct.Pin        = GPIO_PIN_8;
	GPIO_InitStruct.Mode       = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull       = GPIO_NOPULL;
	GPIO_InitStruct.Speed      = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate  = GPIO_AF4_I2C3;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Inertial::InitEXTIGPIO()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// INT1: PA15
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin  = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, IMU_EXTI_NVIC_PRIO, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	// INT2: PC3
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitStruct.Pin  = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI3_IRQn, IMU_EXTI_NVIC_PRIO, 0);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

HAL_StatusTypeDef Inertial::WriteReg(uint8_t addr, uint8_t val)
{
	HAL_StatusTypeDef ret = HAL_I2C_Mem_Write(
		&hi2c,             // HAL I2C handle
		LSM6DSO_I2C_ADD_L, // Device address
		addr,              // Register address
		1,                 // Address size
		&val,              // Tx buffer
		1,                 // Tx buffer size
		1                  // Timeout (HAL tick)
	);

	return ret;
}

HAL_StatusTypeDef Inertial::ReadReg(uint8_t addr, uint8_t* buf)
{
	HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(
		&hi2c,             // HAL I2C handle
		LSM6DSO_I2C_ADD_L, // Device address
		addr,              // Register address
		1,			       // Address size
		buf,     	       // Rx buffer
		1,			       // Rx buffer size
		1			       // Timeout (HAL tick)
	);

	return ret;
}

void Inertial::I2CEventIRQHandler()
{
	HAL_I2C_EV_IRQHandler(&hi2c);
}

void Inertial::I2CErrorIRQHandler()
{
	HAL_I2C_EV_IRQHandler(&hi2c);
}

void Inertial::Int1Handler()
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}

void Inertial::Int2Handler()
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

extern "C" void I2C3_EV_IRQHandler()
{
	Inertial::GetInstance().I2CEventIRQHandler();
}

extern "C" void I2C3_ER_IRQHandler()
{
	Inertial::GetInstance().I2CErrorIRQHandler();
}

extern "C" void EXTI3_IRQHandler()
{
	Inertial::GetInstance().Int2Handler();
}

extern "C" void EXTI15_1_IRQHandler()
{
	Inertial::GetInstance().Int1Handler();
}
