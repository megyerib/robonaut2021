#include "Lsm6dso.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal.h"


stmdev_ctx_t LSM6DSO::device;

LSM6DSO::LSM6DSO()
{
    i2c = INERT_I2C::GetInstance();

    address        = OWN_ADDR;
    controlRegAddr = CONTROL_REG_ADDR;

    initSuccess = false;
}

void LSM6DSO::Init()
{
    // Initialize mems driver interface
    device.write_reg = platform_write;
    device.read_reg  = platform_read;
    device.handle    = i2c->GetHandle();

    // Init test platform.
    platform_init();

    uint8_t pData[5] = {0};
    HAL_I2C_Mem_Read(i2c->GetHandle(), 0xD5, 0x0F, 1, pData, 1, HAL_MAX_DELAY);   // ST = 0x6C

    // Check device ID.
    //whoamI = 0;
    lsm6dso_device_id_get(&device, &whoamI);
    HAL_Delay(2);
    if (whoamI == LSM6DSO_ID)
    {
        // Restore default configuration.
        lsm6dso_reset_set(&device, PROPERTY_ENABLE);
        do
        {
            lsm6dso_reset_get(&device, &rst);
        }
        while (rst);

        // Disable I3C interface
        lsm6dso_i3c_disable_set(&device, LSM6DSO_I3C_DISABLE);

        // Enable Block Data Update
        lsm6dso_block_data_update_set(&device, PROPERTY_ENABLE);

        // Set Output Data Rate
        lsm6dso_xl_data_rate_set(&device, LSM6DSO_XL_ODR_833Hz);
        lsm6dso_gy_data_rate_set(&device, LSM6DSO_GY_ODR_833Hz);

        // Set full scale
        lsm6dso_xl_full_scale_set(&device, LSM6DSO_4g);
        lsm6dso_gy_full_scale_set(&device, LSM6DSO_500dps);

        // Configure filtering chain(No aux interface)
        // Accelerometer - LPF1 + LPF2 path
        lsm6dso_xl_hp_path_on_out_set(&device, LSM6DSO_LP_ODR_DIV_100);
        lsm6dso_xl_filter_lp2_set(&device, PROPERTY_ENABLE);

        initSuccess = true;
    }
    else
    {
        initSuccess = false;
    }
}

void LSM6DSO::Process()
{
    return;
    uint8_t reg;
    GPIO_PinState pinState = GPIO_PIN_RESET;

    //whoamI = 0;
    lsm6dso_device_id_get(&device, &whoamI);
    if ((whoamI == LSM6DSO_ID) && (initSuccess == true))
    {

    }
    else
    {
        //led->TurnOff();
        if (initSuccess == false)
        {
           // i2c->Reset();
          // Init();
        }
    }

    // Read output only if new xl value is available
    lsm6dso_xl_flag_data_ready_get(&device, &reg);
    if (reg)
    {
      // Read acceleration field data
      memset(data_raw_acceleration.u8bit, 0x00, 3 * sizeof(int16_t));
      lsm6dso_acceleration_raw_get(&device, data_raw_acceleration.u8bit);
      acceleration_mg[0] = lsm6dso_from_fs2_to_mg(data_raw_acceleration.i16bit[0]);
      acceleration_mg[1] = lsm6dso_from_fs2_to_mg(data_raw_acceleration.i16bit[1]);
      acceleration_mg[2] = lsm6dso_from_fs2_to_mg(data_raw_acceleration.i16bit[2]);
      pinState = GPIO_PIN_SET;
    }

    lsm6dso_gy_flag_data_ready_get(&device, &reg);
    if (reg)
    {
      // Read angular rate field data
      memset(data_raw_angular_rate.u8bit, 0x00, 3 * sizeof(int16_t));
      lsm6dso_angular_rate_raw_get(&device, data_raw_angular_rate.u8bit);
      angular_rate_mdps[0] = lsm6dso_from_fs2000_to_mdps(data_raw_angular_rate.i16bit[0]);
      angular_rate_mdps[1] = lsm6dso_from_fs2000_to_mdps(data_raw_angular_rate.i16bit[1]);
      angular_rate_mdps[2] = lsm6dso_from_fs2000_to_mdps(data_raw_angular_rate.i16bit[2]);
      pinState = GPIO_PIN_SET;
    }

    lsm6dso_temp_flag_data_ready_get(&device, &reg);
    if (reg)
    {
      // Read temperature data
      memset(data_raw_temperature.u8bit, 0x00, sizeof(int16_t));
      lsm6dso_temperature_raw_get(&device, data_raw_temperature.u8bit);
      temperature_degC = lsm6dso_from_lsb_to_celsius(data_raw_temperature.i16bit);
      pinState = GPIO_PIN_SET;
    }

    UNUSED(pinState);
    //led->SetState(pinState);
}

float LSM6DSO::GetAngular_mdps(Axis const t)
{
    float angular = 0.0f;

    switch (t)
    {
        case Axis::roll:
        {
            angular = angular_rate_mdps[0];
            break;
        }
        case Axis::pitch:
        {
            angular = angular_rate_mdps[1];
            break;
        }
        case Axis::yaw:
        {

            angular = angular_rate_mdps[2];
            break;
        }
        default:
            // Wrong axis.
            break;
    }

    return angular;
}

float LSM6DSO::GetAccel_mg(Axis const t)
{
    float acceleration = 0.0f;

    switch (t)
    {
        case Axis::x:
        {
            acceleration = acceleration_mg[0];
            break;
        }
        case Axis::y:
        {
            acceleration = acceleration_mg[1];
            break;
        }
        case Axis::z:
        {

            acceleration = acceleration_mg[2];
            break;
        }
        default:
            // Wrong axis.
            break;
    }

    return acceleration;
}

void LSM6DSO::OffsetCalibration()
{
    // TODO
}

uint8_t LSM6DSO::GetRandomNumber()
{
    return (uint8_t)((uint32_t)((acceleration_mg[0] + angular_rate_mdps[3] + HAL_GetTick()) * 100) % 255);
}

/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to write
 * @param  bufp      pointer to data to write in register reg
 * @param  len       number of consecutive register to write
 *
 */
int32_t LSM6DSO::platform_write(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
  if (handle == device.handle)
  {
    HAL_I2C_Mem_Write((I2C_HandleTypeDef*)handle, LSM6DSO_I2C_ADD_L, reg, I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
  }
  return 0;
}

/*
 * @brief  Read generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to read
 * @param  bufp      pointer to buffer that store the data read
 * @param  len       number of consecutive register to read
 *
 */
int32_t LSM6DSO::platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
  if (handle == device.handle)
  {
    HAL_I2C_Mem_Read((I2C_HandleTypeDef*)handle, LSM6DSO_I2C_ADD_L, reg, I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
  }
  return 0;
}

/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  tx_buffer     buffer to trasmit
 * @param  len           number of byte to send
 *
 */
void LSM6DSO::tx_com(uint8_t *tx_buffer, uint16_t len)
{
  //HAL_UART_Transmit(&huart2, tx_buffer, len, 1000);
}

/*
 * @brief  platform specific initialization (platform dependent)
 */
void LSM6DSO::platform_init(void)
{
  TIM3->CCR1 = PWM_3V3;
  TIM3->CCR2 = PWM_3V3;
  HAL_Delay(1000);
}




