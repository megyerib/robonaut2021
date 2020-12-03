#include "NavigationTask.h"

#include "TaskPrio.h"

NavigationTask::NavigationTask() : CyclicTask((char*)"Navigation",
                                              NAVIGATION_SAMPLING_CYCLE,
                                              NAVIGATION_TASK_PRIO,
                                              256)
{

}

NavigationTask* NavigationTask::Init()
{
    static NavigationTask instance;
    return &instance;
}

void NavigationTask::TaskInit()
{
	navigation = Navigation::GetInstance();
	navigation->Init();
}

void NavigationTask::TaskFunction()
{
    navigation->Process();
}
