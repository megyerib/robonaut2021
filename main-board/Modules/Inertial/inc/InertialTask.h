#pragma once

#include "CyclicTask.h"

class InertialTask : public CyclicTask
{
public:
    static InertialTask& Init();

private:
    InertialTask();

    virtual void TaskInit() override;
    virtual void TaskFunction() override;
};
