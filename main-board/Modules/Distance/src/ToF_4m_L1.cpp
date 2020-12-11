#include "ToF_4m_L1.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Trace.h"
#include "SerialMessages.h"
#include "Uptime.h"

TOF_L1::TOF_L1()
{
    i2c = ToF_I2c::GetInstance();

    InitXsdnGpio();

    Dev->I2cDevAddr      = 0x52;
    Dev->I2cHandle       = i2c->GetHandle();
    Dev->comms_speed_khz = 100;
    Dev->comms_type      = 1;

    status              = VL53L1_ERROR_NONE;
    timingBudget_ms     = 200;

    XSDN_Port           = TOF_FRONT_XSDN_Port;
    XSDN_Pin            = TOF_FRONT_XSDN_Pin;
}

TOF_L1::TOF_L1(uint8_t             const Addr,
               uint16_t            const Speed,
               uint32_t            const TB_ms,
               GPIO_TypeDef*       const XsdnPort,
               uint16_t            const XsdnPin)
{
    i2c = ToF_I2c::GetInstance();

    InitXsdnGpio();

    Dev->I2cDevAddr      = Addr;
    Dev->I2cHandle       = i2c->GetHandle();
    Dev->comms_speed_khz = Speed;
    Dev->comms_type      = 1;

    status          = VL53L1_ERROR_NONE;
    timingBudget_ms = TB_ms;

    XSDN_Port = XsdnPort;
    XSDN_Pin  = XsdnPin;
}

void TOF_L1::Init()
{
    ShutdownWithDelay();

    if (isAddressForgotten() == true)
    {
        ChangeAddress();
    }

    vTaskDelay(80);

    ConfigureDevice();
}

void TOF_L1::Process()
{
    status = VL53L1_StartMeasurement(Dev);
    status = VL53L1_WaitMeasurementDataReady(Dev);

    if(status == VL53L1_ERROR_NONE)
    {
        // DEB
        HAL_GPIO_WritePin(FREE3_LED_GPIO_Port, FREE3_LED_Pin, GPIO_PIN_SET);

        status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);
        if (status == VL53L1_ERROR_NONE)
        {
            uint16_t millimeter = RangingData.RangeMilliMeter;

            // Trace (dummy)
            /*SM_DUMMY msg;
			msg.id = smDummy;
			msg.timestamp = UPTIME_us();
			msg.value = millimeter;

			TRACE_BIN(&msg, sizeof(msg));*/


            if (millimeter > 300)
            {
                // Turn on green LED
            	//HAL_GPIO_WritePin(FREE1_GPIO_Port, FREE1_Pin, GPIO_PIN_SET);
            }
            else
            {
            	// Turn off green LED
            	//HAL_GPIO_WritePin(FREE1_GPIO_Port, FREE1_Pin, GPIO_PIN_RESET);
            }
        }
        else
        {
        	// Turn off green LED
        	//HAL_GPIO_WritePin(FREE1_GPIO_Port, FREE1_Pin, GPIO_PIN_RESET);
        	PRINTF("ToF error");
        }
        status = VL53L1_ClearInterruptAndStartMeasurement(Dev);
    }
    else
    {
        // DEB
        HAL_GPIO_WritePin(FREE3_LED_GPIO_Port, FREE3_LED_Pin, GPIO_PIN_RESET);
        i2c->Reset();
        Init();
        VL53L1DevDataSet(Dev, PalState, VL53L1_STATE_IDLE);
        VL53L1_StopMeasurement(Dev);
    }
}

uint16_t TOF_L1::GetDistance_mm(void)
{
    return RangingData.RangeMilliMeter;
}

void TOF_L1::Calibrate_Offset_300mm(void)
{
    VL53L1_PerformOffsetCalibration(Dev, 300);
}

void TOF_L1::InitXsdnGpio()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // GPIO Ports Clock Enable
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Configure GPIO pin Output Level
    HAL_GPIO_WritePin(GPIOC, TOF_FRONT_XSDN_Pin|TOF_XSDN2_Pin, GPIO_PIN_RESET);

    // Configure GPIO pins : PCPin PCPin PCPin PCPin
    GPIO_InitStruct.Pin     = TOF_FRONT_XSDN_Pin|TOF_XSDN2_Pin;
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

bool TOF_L1::isAddressForgotten()
{
    bool isForgotten = true;
//    uint16_t newAddr = Dev->I2cDevAddr;

    // Try to reach the device with the desired custom I2C Address.
    if (isDeviceConnected() == true)
    {
        isForgotten = false;
    }
//    else
//    {
//        // Try to reach at the default I2C Address.
//        Dev->I2cDevAddr = 0x52;
//        if (isDeviceConnected() == true)
//        {
//            isForgotten = false;
//        }
//        else
//        {
//            // Not found on the default address
//        }
//        Dev->I2cDevAddr = newAddr;
//    }

    return isForgotten;
}

void TOF_L1::ShutdownOn()
{
    HAL_GPIO_WritePin(XSDN_Port, XSDN_Pin, GPIO_PIN_RESET);
}

void TOF_L1::ShutdownOff()
{
    HAL_GPIO_WritePin(XSDN_Port, XSDN_Pin, GPIO_PIN_SET);
}

void TOF_L1::ShutdownWithDelay()
{
    ShutdownOn();
    vTaskDelay(150);
    ShutdownOff();
}

void TOF_L1::ChangeAddress()
{
    uint16_t newAddr = Dev->I2cDevAddr;

    // Reach the device with the default sensor I2C address.
    Dev->I2cDevAddr = 0x52;

    if (isDeviceConnected() == true)
    {
        status = VL53L1_SetDeviceAddress(Dev, newAddr);
        Dev->I2cDevAddr = newAddr;
    }
}

bool TOF_L1::isDeviceConnected()
{
    bool deviceIsconnected = false;
    uint16_t wordData = 0x00;

    // Test I2C interface.
    status = VL53L1_RdWord(Dev, 0x010F, &wordData);
    if ((status == VL53L1_ERROR_NONE) && (wordData == 0xEACC))
    {
        deviceIsconnected = true;
    }

    return deviceIsconnected;
}

void TOF_L1::ConfigureDevice()
{
    if (isDeviceConnected() == true)
    {
        status = VL53L1_WaitDeviceBooted(Dev);
        status = VL53L1_DataInit(Dev);
        status = VL53L1_StaticInit(Dev);
        status = VL53L1_SetDistanceMode(Dev, VL53L1_DISTANCEMODE_LONG);
        status = VL53L1_SetMeasurementTimingBudgetMicroSeconds(Dev, timingBudget_ms * 100);
        status = VL53L1_SetInterMeasurementPeriodMilliSeconds(Dev, timingBudget_ms);
        status = VL53L1_StartMeasurement(Dev);
    }
    else
    {
        status = VL53L1_ERROR_CONTROL_INTERFACE;
    }
}

