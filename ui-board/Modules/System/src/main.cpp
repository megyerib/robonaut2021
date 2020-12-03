#include "UiTask.h"
#include "System.h"

int main(void)
{
	SYSTEM_INIT();

	UiTask::Init();

	OS_START();

	return 0;
}
