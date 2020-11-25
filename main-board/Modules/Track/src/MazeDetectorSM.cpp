#include "MazeDetectorSM.h"
#include "Encoder.h"
#include <cstdio>

MazeDetectorSM::MazeDetectorSM(LineData& line) : line(line)
{

}

void MazeDetectorSM::Process()
{
	(this->*stateMatrix[state][line.lType])();
}

void MazeDetectorSM::NewSection()
{
	Encoder* e = Encoder::GetInstance();

	prevSectionLen = e->GetDistance() - sectionStart;
	sectionStart = e->GetDistance();

	/*char buf[30];
	int strlen = sprintf(buf, "%d", (int)(prevSectionLen*100));
	trace->Transmit(buf, strlen);*/

	if (state != td_1 && state != td_2Near)
	{
		doubleStripes = 0;
	}
}

void MazeDetectorSM::RegFork2()
{
	state = td_2Fork;
	NewSection();

	type  = Fork2;
	dir   = ld_Middle;
}

void MazeDetectorSM::RegFork3()
{
	state = td_3Fork;
	NewSection();

	type  = Fork3;
	dir   = ld_Middle;
}

void MazeDetectorSM::RegJct2()
{
	state = td_2Junction;
	NewSection();

	type  = Junction2;

	switch (line.chosenIndex)
	{
	case 0:
		dir = ld_Left;
		break;
	case 1:
		dir = ld_Right;
		break;
	default:
		break;
	}
}

void MazeDetectorSM::RegJct3()
{
	state = td_3Junction;
	NewSection();

	type  = Junction3;

	switch (line.chosenIndex)
	{
	case 0:
		dir = ld_Left;
		break;
	case 1:
		dir = ld_Middle;
		break;
	case 2:
		dir = ld_Right;
		break;
	default:
		break;
	}
}

void MazeDetectorSM::Reg1()
{
	state = td_1;
	NewSection();

	type  = Single;
	dir   = ld_Middle;
}

void MazeDetectorSM::RegEnd()
{
	state = td_End;
	NewSection();

	type  = DeadEnd;
	dir   = ld_Middle;
}

void MazeDetectorSM::RegExit()
{
	state = td_1;
	NewSection();

	if (doubleStripes == 1)
	{
		if (prevSectionLen > 0.12f) // Middle stripe length
		{
			type = Exit;
		}
		else if (prevSectionLen > 0.05f) // Plausibility check (noise is smaller than 5 cm)
		{
			type = ExitReverse;
		}
		else
		{
		    return; // Noise
		}

		if (lineIndex == 0) // We're going on the left
		{
			dir = ld_Right;
			//trace->Transmit("Right", 5);
		}
		else
		{
			dir = ld_Left;
			//trace->Transmit("Left", 4);
		}
	}

	if (doubleStripes == 5)
	{
		type = Single;
		doubleStripes = 0; // reset
	}
}

void MazeDetectorSM::Go2Near()
{
	state = td_2Near;
	doubleStripes++;
	lineIndex = line.chosenIndex;

	NewSection();
}

void MazeDetectorSM::Go2Far()
{
	state = td_2Far;
	NewSection();
}

void MazeDetectorSM::Go3Near()
{
	state = td_3Near;
	NewSection();
}

void MazeDetectorSM::Go3Far()
{
	state = td_3Far;
	NewSection();
}
