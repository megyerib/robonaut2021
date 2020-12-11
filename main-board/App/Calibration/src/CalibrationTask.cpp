#include "CalibrationTask.h"
#include "Remote.h"
#include "TraceTask.h"
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
	TraceTask::Init();

	sensorServo.SetSteerAngle(1.57f);
}

void CalibrationTask::TaskFunction()
{
	Remote& remote  = Remote::GetInstance();

	float steering  = remote.GetValue(chSteering);
	float throttle  = remote.GetValue(chThrottle);
	//RemoteMode mode = remote.GetMode();

	if (fabs(steering) > 0.1f)
	{
		steeringCompare -= steering * 10; // Reverse
		SATURATE(steeringCompare, 1000, 2000);

		wait.Wait_s(1);
	}

	if (wait.IsExpired())
	{
		PRINTF("Steering: %d", steeringCompare);
		//PRINTF("Sensor:   %d", sensorCompare);
		wait.Stop();
	}

	Traction::GetInstance()->SetDutyCycle(throttle * THROTTLE_FUN_FACTOR);

	steeringServo.SetPulseWidth_us(steeringCompare);
	//sensorServo.SetPulseWidth_us(sensorCompare);
}
