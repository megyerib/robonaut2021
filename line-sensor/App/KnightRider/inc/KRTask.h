#pragma once

#include "CyclicTask.h"
#include <cstdint>

// Knight Rider light bar task

class KRTask : CyclicTask
{
public:
	static KRTask& Init();

private:
	KRTask();

	void TaskInit() override;
	void TaskFunction() override;

	void KnightRiderCycle(uint32_t& pattern, bool& dir);

	bool     ledDirLeft = true;
	uint32_t ledPattern = 0;

	bool     irDirLeft = true;
	uint32_t irPattern = 0;
};
