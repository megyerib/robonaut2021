#if 1

#define __weak   __attribute__((weak))

void OsError()
{
	int proceed = 0;

	while (!proceed); // Set breakpoint here
}

__weak void vApplicationMallocFailedHook (void)
{
	OsError();
}

#endif
