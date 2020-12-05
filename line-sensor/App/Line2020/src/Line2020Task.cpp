#include "Line2020Task.h"
#include "SensorMeasure.h"
#include "ShiftReg.h"
#include "StddevEval.h"

#include "Display.h"
#include "SensorDriver.h"

#include "SensorCfg.h"
#include "SensorSettings.h"

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
	SensorMeasure* adc = SensorMeasure::GetInstance();
	SensorDriver& sensors = SensorDriver::GetInstance();
	Display& d = Display::GetInstance();
	StddevEval eval;

	static AdcMeasType measurements[SENSOR_SIZE];
	static uint32_t cycle = 0;
	static uint32_t cyclesDone = 0;

	adc->Measure((AdcInput)(cycle+0));
	adc->Measure((AdcInput)(cycle+IR_GROUP_SIZE));

	if (cycle == (IR_GROUP_SIZE - 1))
	{
		adc->GetMeasurements(measurements);

		eval.Feed(measurements);
		LineInput l = eval.GetLine();

		if (cyclesDone > 0) // Don't send first line
		{
			d.DisplayLinePos(l);
			//comm->SendLine(&l);
		}

		cycle = 0;
		cyclesDone++;
	}
	else
	{
		cycle++;
	}

	sensors.SetSensors(IR_GROUP_SIZE, cycle);
}
