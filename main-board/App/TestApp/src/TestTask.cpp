#include "../../TestApp/inc/TestTask.h"

#include "trace.h"

#define PRIO       6
#define PERIOD  1000
#define STACK    128

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

}

void TestTask::TaskFunction()
{
	static int i = 0;

	//PRINTF("Line %d", i);
	//PRINTF("Hey!");
	i++;
}
