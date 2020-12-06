#include "IrDriver.h"
#include "LedDriver.h"
#include "Line2020Task.h"
#include "SensorMeasure.h"
#include "ShiftReg.h"
#include "StddevEval.h"
#include "BinTrace.h"
#include "SensorCfg.h"
#include "SensorSettings.h"
#include "MainLsMsg.h"

#define PRIO                 2
#define PERIOD               1
#define STACK              128

Line2020Task::Line2020Task() : CyclicTask((char*)"Line2020", PERIOD, PRIO, STACK)
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
	static LedDriver& d       = LedDriver::GetInstance();
	static StddevEval eval;

	static AdcMeasType measurements[SENSOR_SIZE];
	static uint32_t cycle = 0;
	static uint32_t cyclesDone = 0;

	adc.Measure((AdcInput)(cycle+0));
	adc.Measure((AdcInput)(cycle+IR_GROUP_SIZE));

	if (cycle == (IR_GROUP_SIZE - 1))
	{
		adc.GetMeasurements(measurements);

		eval.Feed(measurements);
		LineInput l = eval.GetLine();

		if (cyclesDone > 0) // Don't send first line
		{
			d.DisplayLinePos(l);

			L2M_LINE_DATA msg;

			msg.line = l;
			TRACE_BIN(&msg, sizeof(msg));

			trace.Process(); // Could be placed elsewhere
		}

		cycle = 0;
		cyclesDone++;

		// TODO time meas
	}
	else
	{
		cycle++;
	}

	// Set the sensors to the next cycle
	sensors.SetSensors(IR_GROUP_SIZE, cycle);
}
