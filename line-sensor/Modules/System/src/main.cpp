#include "System.h"
#include "KRTask.h"
#include "Line2020Task.h"

// Clean project after changing sensor revision or size!
// (the project contains macros defined in the compiler parameters)

int main(void)
{
	SYSTEM_INIT();

	Line2020Task::Init();

	OS_START();

	return 0;
}
