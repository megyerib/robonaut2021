#include "TestTask.h"
#include "TraceTask.h"
#include "InertialTask.h"

#define PRIO        5
#define PERIOD      5
#define STACK    1024

TestTask::TestTask() : CyclicTask((char*)"Test", PERIOD, PRIO, STACK)
{

}

TestTask& TestTask::Init()
{
	static TestTask instance;
	return instance;
}

void TestTask::TaskInit()
{
	TraceTask::Init();
	InertialTask::Init();
}

void TestTask::TaskFunction()
{

}
