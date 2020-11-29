#include "LineDetectorLoop.h"

#include "SensorMeasure.h"
#include "ShiftReg.h"
#include "StddevEval.h"

#include "Display.h"
#include "SensorDriver.h"

#include "SensorCfg.h"
#include "SensorSettings.h"
#include "Comm.h"

void LineDetectorLoop::Run()
{
	SensorMeasure* adc = SensorMeasure::GetInstance();
	SensorDriver sensors;
	StddevEval eval;
	Display d;
	Comm* comm = Comm::GetInstance();

	AdcMeasType measurements[SENSOR_SIZE];
	LineInput l;

	while (1)
	{
		for (int i = 0; i < IR_GROUP_SIZE; i++)
		{
			sensors.SetSensors(IR_GROUP_SIZE, i);
			HAL_Delay(1); // TODO us delay

			adc->Measure((AdcInput)(i+0));
			adc->Measure((AdcInput)(i+IR_GROUP_SIZE));
		}

		adc->GetMeasurements(measurements);

		eval.Feed(measurements);
		l = eval.GetLine();
		d.DisplayLinePos(l);

		comm->SendLine(&l);
	}
}
