#include "MusicTask.h"
#include "Tone.h"
#include "TractionTask.h"
#include "PiFoxboro_Controller.h"
#include "Encoder.h"
#include "TraceTask.h"
#include "Trace.h"
#include "SerialMessages.h"
#include "Uptime.h"

#define PRIO        5
#define PERIOD     10
#define STACK    configMINIMAL_STACK_SIZE

#define HZ_TO_MPS  0.003333333f

static const PIFB_PARAMS foxboroParams =
{
	.T_s     =  0.01f,
	.T_Plant =  0.55f,
	.K_Plant = 14.78f,
	.u_MIN   = -0.4f,
	.u_MAX   =  0.4f,
	.T_Cl    =  0.25f
};

static PiFoxboro_Controller controller(foxboroParams);

MusicTask::MusicTask() : CyclicTask((char*)"Music", PERIOD, PRIO, STACK)
{

}

MusicTask& MusicTask::Init()
{
	static MusicTask instance;
	return instance;
}

void MusicTask::TaskInit()
{
	TractionTask::Init();
	TraceTask::Init();
	EncoderTask::Init();

	Traction::GetInstance()->SetMode(tmDutyCycle);
	Traction::GetInstance()->SetDutyCycle(0);
}

void MusicTask::TaskFunction()
{
	static uint32_t counter = 0;

	TONE tonesToPlay[] = {Fs5, Fs5, Fs5, Cs6, B5, B5, A5, Gs5, Gs5, Gs5, B5, B5, A5, Gs5};
	size_t toneArrSize = sizeof(tonesToPlay) / sizeof(tonesToPlay[0]);

	size_t indexToPlay = (PERIOD * counter / 250) % toneArrSize;
	TONE toneToPlay = tonesToPlay[indexToPlay];
	float freqToPlay = toneFreq[toneToPlay];

	float refSpeed  = freqToPlay * HZ_TO_MPS;
	float measSpeed = Encoder::GetInstance().GetSpeed();

	TRACE_DUMMY(measSpeed * 1000);

	controller.SetRef(refSpeed);
	controller.Process(measSpeed);

	float d = controller.GetOutput();
	Traction::GetInstance()->SetDutyCycle(d);

	counter++;
}
