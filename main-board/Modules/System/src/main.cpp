#include "System.h"
#include "TestTask.h"

int main()
{
	SYSTEM_INIT();

	TestTask::Init();

	OS_START();

	while(1);

    return 0;
}
