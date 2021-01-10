#pragma once

#include "CyclicTask.h"

class TrackTask : public CyclicTask
{
	TrackTask();

	void TaskInit() override;
	void TaskFunction() override;

public:
	static TrackTask* Init();
};
