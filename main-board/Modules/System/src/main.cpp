#include "System.h"

#include "DistanceTask.h"
#include "CalibrationTask.h"
#include "RcTask.h"

#include "Q1Task.h"

int main()
{
	SYSTEM_INIT();

	Q1Task::Init();

	OS_START();

	while(1);

    return 0;
}
