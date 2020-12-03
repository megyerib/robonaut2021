#include "Ui2020Task.h"
#include "System.h"

int main(void)
{
	SYSTEM_INIT();

	Ui2020Task::Init();

	OS_START();

	return 0;
}
