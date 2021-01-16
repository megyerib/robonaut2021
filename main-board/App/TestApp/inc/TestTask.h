#pragma once

#include "CyclicTask.h"
#include "mapnavigation.h"

class TestTask : public CyclicTask
{
public:
	static TestTask& Init();

private:
	TestTask();

	void TaskInit() override;
	void TaskFunction() override;

	MapNavigation navi;
	uint8_t       target_node;
	MAZE_MOVE     move;
	MAZE_SECTION  current_section;
	MAZE_SECTION  source_section;
};
