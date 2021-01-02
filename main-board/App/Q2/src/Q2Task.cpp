#include "Q2Task.h"
#include "MazeNav_AlwaysLeft.h"

#define PRIO        5
#define PERIOD      5
#define STACK    1024

Q2Task::Q2Task() : CyclicTask((char*)"Q2", PERIOD, PRIO, STACK)
{

}

Q2Task& Q2Task::Init()
{
	static Q2Task instance;
	return instance;
}

void Q2Task::TaskInit()
{

}

void Q2Task::TaskFunction()
{

}
