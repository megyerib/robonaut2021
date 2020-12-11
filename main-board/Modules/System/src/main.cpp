#include "System.h"

#include "TestTask.h"
#include "DistanceTask.h"
#include "CalibrationTask.h"

int main()
{
	SYSTEM_INIT();

	CalibrationTask::Init();

	OS_START();

	while(1);

    return 0;
}
