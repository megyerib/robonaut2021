#include "TestLoop.h"
#include "System.h"
#include "LineDetectorLoop.h"

int main(void)
{
	System::Init();

	LineDetectorLoop::Run();
	//TestLoop::RunLineTest();
}

// TODO Időmérés (usec)
// TODO ütemező
// TODO NVIC prioritások
// TODO minden singleton
