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

	navi.InitMap();
	target_node = 13U;
	current_section = sStpos;
	source_section = sStpos;
}

void TestTask::TaskFunction()
{
    navi.SetCurrentSection(source_section);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(target_node);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(target_node);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(target_node);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(target_node);
    current_section = navi.GetCurrentSection();
    move = navi.GetNextMove(target_node);
}
