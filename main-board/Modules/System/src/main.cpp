#include "System.h"

#include "MusicTask.h"

int main()
{
	SYSTEM_INIT();

	//MusicTask::Init();

	OS_START();

	while(1);

    return 0;
}
