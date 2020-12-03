#pragma once

#include "Inert_I2c.h"
#include "lsm6dso_reg.h"

#define PWM_3V3             ( 915)
#define OWN_ADDR            (0XD5)
#define CONTROL_REG_ADDR    (0x0F)
#define CONTROL_REG_VALUE   (0x6C)

typedef enum
{
    x = 0,
    y,
    z,
    roll,
    pitch,
    yaw
} Axis;

typedef union
{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} axis3bit16_t;

typedef union
{
  int16_t i16bit;
  uint8_t u8bit[2];
} axis1bit16_t;

class LSM6DSO
{
public:
    LSM6DSO();

    void Init();
    void Process();

    float GetAngular_mdps(Axis const t);
    float GetAccel_mg(Axis const t);
    void  OffsetCalibration();

    uint8_t GetRandomNumber();

private:    // I2C interface
    INERT_I2C*      i2c;
    uint8_t         address;
    uint8_t         controlRegAddr;

private:    // LSM6DSO components
    static stmdev_ctx_t device;
    bool initSuccess;

    axis3bit16_t    data_raw_acceleration;
    axis3bit16_t    data_raw_angular_rate;
    axis1bit16_t    data_raw_temperature;
    float           acceleration_mg[3];
    float           angular_rate_mdps[3];
    float           temperature_degC = 0;
    uint8_t         whoamI = 0;
    uint8_t         rst = 0;
    uint8_t         tx_buffer[1000];

// Platform dependent functions.
    static int32_t platform_write(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);
    static int32_t platform_read (void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);
    static void    tx_com        (uint8_t *tx_buffer, uint16_t len);
    static void    platform_init (void);

private:

};
