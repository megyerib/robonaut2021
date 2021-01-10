#include "TrackTask.h"
#include "TaskPrio.h"
#include "TrackDetector.h"

TrackTask::TrackTask() : CyclicTask((char*)"Track", LINE_SAMPLING_CYCLE, TRACK_TASK_PRIO, configMINIMAL_STACK_SIZE)
{

}

TrackTask* TrackTask::Init()
{
	static TrackTask instance;
	return &instance;
}

void TrackTask::TaskInit()
{

}

void TrackTask::TaskFunction()
{
	TrackDetector::GetInstance()->Process();
}
