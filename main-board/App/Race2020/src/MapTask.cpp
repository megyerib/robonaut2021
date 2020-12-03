#include "MapTask.h"

#include "TaskPrio.h"


MapTask::MapTask() : CyclicTask((char*)"Map",
                                MAP_SAMPLING_PERIOD,
                                MAP_TASK_PRIO,
                                1024)
{

}

MapTask* MapTask::Init()
{
    static MapTask instance;
    return &instance;
}

void MapTask::TaskInit()
{
    map = Map::GetInstance();
}

void MapTask::TaskFunction()
{
    map->Process();
}
