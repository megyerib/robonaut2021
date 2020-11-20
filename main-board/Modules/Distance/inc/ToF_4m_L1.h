#pragma once

#include "vl53l1_api.h"
#include "vl53l1_platform.h"
#include "vl53l1_def.h"

#include "Tof_I2c.h"
#include "Transmitter.h"

#define TOF_FRONT_XSDN_Pin  GPIO_PIN_15
#define TOF_FRONT_XSDN_Port GPIOC
#define TOF_XSDN2_Pin       GPIO_PIN_13
#define TOF_XSDN2_Port      GPIOC

#define FREE3_LED_Pin       GPIO_PIN_3
#define FREE3_LED_GPIO_Port GPIOB
#define FREE2_Pin           GPIO_PIN_4
#define FREE2_GPIO_Port     GPIOB
#define FREE1_Pin           GPIO_PIN_5
#define FREE1_GPIO_Port     GPIOB

class TOF_L1
{
private:
    ToF_I2c*        i2c;

    VL53L1_Dev_t    dev;
    VL53L1_DEV      Dev = &dev;

    VL53L1_Error    status;
    uint32_t        timingBudget_ms;
    VL53L1_RangingMeasurementData_t RangingData;

    GPIO_TypeDef*   XSDN_Port;
    uint16_t        XSDN_Pin;

public:
    TOF_L1();
    TOF_L1(uint8_t              const Addr,
           uint16_t             const Speed,
           uint32_t             const TB_ms,
           GPIO_TypeDef*        const XsdnPort,
           uint16_t             const XsdnPin);

    void Init();
    void Process();

    uint16_t GetDistance_mm(void);

    void Calibrate_Offset_300mm(void);

private:
    void InitXsdnGpio();
    bool isAddressForgotten();
    void Shutdown();
    void Startup();
    void Restart();
    void ChangeAddress();
    bool isDeviceConnected();
    void ConfigureDevice();
};
