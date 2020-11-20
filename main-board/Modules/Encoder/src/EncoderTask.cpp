#include "EncoderTask.h"

#define PERIOD     ENCODER_SAMPLING_PERIOD
#define PRIO       7
#define STACK      configMINIMAL_STACK_SIZE

EncoderTask::EncoderTask() : CyclicTask((char*)"Encoder", ENCODER_SAMPLING_PERIOD, PRIO, STACK)
{

}

EncoderTask* EncoderTask::Init()
{
	static EncoderTask instance;
	return &instance;
}

void EncoderTask::TaskInit()
{
	encoder = Encoder::GetInstance();
}

void EncoderTask::TaskFunction()
{
	encoder->Process();
}
