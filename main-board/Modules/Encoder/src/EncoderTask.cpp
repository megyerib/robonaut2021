#include "EncoderTask.h"
#include "Encoder.h"

#define PERIOD     ENC_SAMPLING_PERIOD_ms
#define PRIO       3
#define STACK      configMINIMAL_STACK_SIZE

EncoderTask::EncoderTask() : CyclicTask((char*)"Enc", PERIOD, PRIO, STACK)
{

}

EncoderTask& EncoderTask::Init()
{
    static EncoderTask instance;
    return instance;
}

void EncoderTask::TaskInit()
{

}

void EncoderTask::TaskFunction()
{
	Encoder::GetInstance().Process();
}
