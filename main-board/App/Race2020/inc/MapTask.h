#pragma once

#include "CyclicTask.h"

#include "Map.h"

class MapTask : public CyclicTask
{
public:
    static MapTask* Init();

private:
    Map* map = nullptr;

    MapTask();

    void TaskInit() override;
    void TaskFunction() override;
};

