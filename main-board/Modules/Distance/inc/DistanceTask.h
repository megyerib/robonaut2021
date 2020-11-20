#pragma once

#include "CyclicTask.h"
#include "Distance.h"

class DistanceTask : public CyclicTask
{
    Distance* distance = nullptr;

    DistanceTask();

    void TaskInit() override;
    void TaskFunction() override;

public:
    static DistanceTask* Init();
};
