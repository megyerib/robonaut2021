#include <KRTask.h>
#include "Display.h"
#include "SensorDriver.h"

#define PRIO                 3
#define PERIOD              25
#define STACK              configMINIMAL_STACK_SIZE

#define KNIGHT_RIDER_WIDTH   5

KRTask::KRTask() : CyclicTask((char*)"KR", PERIOD, PRIO, STACK)
{

}

KRTask& KRTask::Init()
{
	static KRTask instance;
	return instance;
}

void KRTask::TaskInit()
{
	ledDirLeft = true;
	ledPattern = 0xFFFFFFFF >> (SENSOR_SIZE - KNIGHT_RIDER_WIDTH);

	irDirLeft = true;
	irPattern = 1;
}

void KRTask::TaskFunction()
{
	// LED (Knight Rider)
	Display::GetInstance().DisplayPattern(ledPattern);
	KnightRiderCycle(ledPattern, ledDirLeft);

	// IR (only 1)
	KnightRiderCycle(irPattern, irDirLeft);
	SensorDriver::GetInstance().DisplayPattern(irPattern);
}

void KRTask::KnightRiderCycle(uint32_t& pattern, bool& dirLeft)
{
	if (dirLeft)
		pattern <<= 1;
	else
		pattern >>= 1;

	if ((pattern & 1) != 0)
		dirLeft = true;

	if ((pattern & (1 << (SENSOR_SIZE - 1))) != 0)
		dirLeft = false;
}
