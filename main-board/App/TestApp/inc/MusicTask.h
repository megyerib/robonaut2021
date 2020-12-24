#pragma once

#include "CyclicTask.h"

class MusicTask : public CyclicTask
{
public:
	static MusicTask& Init();

private:
	MusicTask();

	void TaskInit() override;
	void TaskFunction() override;
};
