#include <Q1Task2020.h>
#include "TaskPrio.h"

#define CYCLE_TIME 16 /* Remote reading only */

#define RC_THROTTLE_THRESHOLD    (0.1f)
#define RC_THROTTLE_FUN_FACTOR   (0.4f)

Q1Task2020::Q1Task2020() : CyclicTask((char*)"Q1", CYCLE_TIME, MAIN_TASK_PRIO, 1024),
                   remote(Remote::GetInstance())
{

}

Q1Task2020* Q1Task2020::Init()
{
	static Q1Task2020 instance;
	return &instance;
}

void Q1Task2020::TaskInit()
{
	car      = Car::GetInstance();
	motor    = Traction::GetInstance();
	steering = Steering::GetInstance();
}

void Q1Task2020::TaskFunction()
{
	float      throttle   = remote.GetValue(chThrottle);
	float      steerAngle = remote.GetValue(chSteering);
	RemoteMode mode       = remote.GetMode();

	if (mode == RemMode1)
	{
		motor->SetMode(tmode_Controller);
	    car->SetSteeringMode(SteeringMode::SingleLineFollow_Slow);
		car->StateMachine();
	}

	if (mode == RemMode2)
	{
	    motor->SetMode(tmode_Manual);
	    steering->SetMode(Manual);
		RcRun(throttle, steerAngle);
	}
}

void Q1Task2020::RcRun(float throttle, float steerAngle)
{
	if (throttle > RC_THROTTLE_THRESHOLD || throttle < -RC_THROTTLE_THRESHOLD)
	{
		motor->SetDutyCycle(throttle * RC_THROTTLE_FUN_FACTOR); // Don't use the full scale
	}
	else
	{
		motor->SetDutyCycle(0);
	}

	steering->SetAngleManual(-steerAngle); // , steerAngle);
}