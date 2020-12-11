#include "CalibrationTask.h"
#include "Remote.h"
#include "Trace.h"
#include "TractionTask.h"
#include "math.h"

#define PRIO       6
#define PERIOD    20
#define STACK     configMINIMAL_STACK_SIZE

#define THROTTLE_FUN_FACTOR  (0.3f)

#define SATURATE(x, min, max)  ((x) = (x) > (max) ? (max) : ((x) < (min) ? (min) : (x)))

CalibrationTask::CalibrationTask() : CyclicTask((char*)"Calib", PERIOD, PRIO, STACK),
                                     steeringServo(eTIM12, TIM_CHANNEL_2),
									 sensorServo(eTIM8, TIM_CHANNEL_1)
{

}

CalibrationTask& CalibrationTask::Init()
{
	static CalibrationTask instance;
	return instance;
}

void CalibrationTask::TaskInit()
{
	TractionTask::Init();
}

void CalibrationTask::TaskFunction()
{
	Remote& remote  = Remote::GetInstance();

	float steering  = remote.GetValue(chSteering);
	float throttle  = remote.GetValue(chThrottle);
	RemoteMode mode = remote.GetMode();

	if (fabs(steering) > 0.1f)
	{
		if (mode == RemMode1)
		{
			sensorCompare += steering * 10;
			SATURATE(sensorCompare, 1000, 2000);
		}
		else
		{
			steeringCompare += steering * 10;
			SATURATE(steeringCompare, 1000, 2000);
		}
	}

	// TODO Trace

	Traction::GetInstance()->SetDutyCycle(throttle * THROTTLE_FUN_FACTOR);

	steeringServo.SetPulseWidth_us(steeringCompare);
	sensorServo.SetPulseWidth_us(sensorCompare);
}
