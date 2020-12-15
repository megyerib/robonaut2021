#include "IrDriver.h"
#include "LedDriver.h"
#include "Line2020Task.h"
#include "SensorMeasure.h"
#include "ShiftReg.h"
#include "StddevEval.h"
#include "BinTrace.h"
#include "SensorCfg.h"
#include "MainLsMsg.h"
#include <functional>
#include "MainUart.h"
#include "LineFilter.h"

#define PRIO                 2
#define PERIOD               1
#define STACK              128

#define RX_CYCLE            10
#define FILTER_SIZE          5 /* Don't change it */

Line2020Task::Line2020Task() : CyclicTask((char*)"Line2020", PERIOD, PRIO, STACK),
                               cmdProc(MainUart::GetInstance(), cmdProcBuf, CMD_PROC_BUF_LEN, &enc)
{

}

Line2020Task& Line2020Task::Init()
{
	static Line2020Task instance;
	return instance;
}

void Line2020Task::TaskInit()
{

}

void Line2020Task::TaskFunction()
{
	static BinTrace& trace    = BinTrace::GetInstance();
	static SensorMeasure& adc = SensorMeasure::GetInstance();
	static IrDriver& sensors  = IrDriver::GetInstance();
	static StddevEval eval;

	static AdcMeasType measurements[SENSOR_SIZE];
	static uint32_t cycle = 0;

	static LineInput meas[FILTER_SIZE];
	static size_t measIndex = 0;

	adc.Measure((AdcInput)(cycle+0));
	adc.Measure((AdcInput)(cycle+IR_GROUP_SIZE));

	if (cycle == (IR_GROUP_SIZE - 1))
	{
		adc.GetMeasurements(measurements);

		eval.Feed(measurements);
		meas[measIndex] = eval.GetLine();

		LineInput resultLine = FilterLines(meas, measIndex);
		measIndex = (measIndex + 1) % FILTER_SIZE;

		LedDriver::GetInstance().DisplayLinePos(resultLine);

		if (lineDataEn && measEn)
		{
			L2M_LINE_DATA msg;
			msg.line = resultLine;
			TRACE_BIN(&msg, sizeof(msg));
		}

		if (sensorDataEn && measEn)
		{
			TraceSensorData(measurements);
		}

		trace.Process(); // Could be placed elsewhere

		cycle = 0;
	}
	else
	{
		cycle++;
	}

	// Set the sensors to the next cycle
	sensors.SetSensors(IR_GROUP_SIZE, cycle);

	static uint8_t i = 0;

	if (i == RX_CYCLE)
	{
		ProcessRxQueue();
		i = 0;
	}
	else
	{
		i++;
	}
}

void Line2020Task::TraceSensorData(uint32_t data[SENSOR_SIZE])
{
	static uint8_t buf[sizeof(L2M_SENSOR_DATA) + SENSOR_SIZE * sizeof(uint16_t)];

	L2M_SENSOR_DATA* msg = (L2M_SENSOR_DATA*)buf;
	msg->ID = l2mSensorData;
	msg->size = SENSOR_SIZE;

	for (int i = 0; i < SENSOR_SIZE; i++)
	{
		msg->data[i] = data[i];
	}

	TRACE_BIN(buf, sizeof(buf));
}

void Line2020Task::ProcessRxQueue()
{
	uint8_t buf[20];
	size_t size;

	while (true)
	{
		cmdProc.Receive(buf, size, 20);

		if (size > 0)
		{
			ProcessRxMessage(buf, size);
		}
		else
		{
			break;
		}
	}
}

#define TYPE_SIZE_ASSERT(x) if (size != sizeof(x)) return

void Line2020Task::ProcessRxMessage(uint8_t buf[], size_t size)
{
	static const std::function<void()> MsgProc[m2lCnt] =
	{
		[m2lReset] = [&](){ TYPE_SIZE_ASSERT(M2L_RESET);
			                NVIC_SystemReset();},

		[m2lCfg]   = [&](){ TYPE_SIZE_ASSERT(M2L_CFG);
		                    M2L_CFG* msg = (M2L_CFG*)buf;
						    lineDataEn   = msg->LineDataEn;
			                sensorDataEn = msg->SensorDataEn;
			                measEn       = msg->MeasEn;
			                ledEn        = msg->LedEn;

			                LedDriver::GetInstance().Enable(ledEn);
			                IrDriver::GetInstance().Enable(measEn);
			                },
	};

	if (size > 0)
	{
		uint8_t ID = buf[0];

		if (ID < m2lCnt)
		{
			MsgProc[ID]();
		}
	}
}
