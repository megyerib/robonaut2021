#pragma once

#include "TrackDetectorTypes.h"

#define P(x) &MazeDetectorSM::x

#define REG_FORK_2 &MazeDetectorSM::RegFork2
#define REG_FORK_3 &MazeDetectorSM::RegFork3
#define REG_JCT_2_ &MazeDetectorSM::RegJct2
#define REG_JCT_3_ &MazeDetectorSM::RegJct3
#define REG_EXIT__ &MazeDetectorSM::RegExit
#define REG_1_____ &MazeDetectorSM::Reg1
#define REG_END___ &MazeDetectorSM::RegEnd
#define GO_2_NEAR_ &MazeDetectorSM::Go2Near
#define GO_2_FAR__ &MazeDetectorSM::Go2Far
#define GO_3_NEAR_ &MazeDetectorSM::Go3Near
#define GO_3_FAR__ &MazeDetectorSM::Go3Far
#define ERROR_____ &MazeDetectorSM::Error
#define NONE______ &MazeDetectorSM::None

class MazeDetectorSM
{
public:
	MazeDetectorSM(LineData& line);

	void Process();

	TrackType GetTrackType(){return type;}
	LineDirection GetLineDirection(){return dir;}

private:
	LineData&          line;
	TrackDetectorState state = td_1;
	TrackType          type  = Single;
	LineDirection      dir   = ld_Middle;

	float       sectionStart = 0.0f;
	float     prevSectionLen = 0.0f;

	uint32_t   doubleStripes = 0;
	uint32_t       lineIndex = 0;

	void (MazeDetectorSM::*stateMatrix[td_Num][lt_Num])() =
	{
		//                lt_1,       lt_2Near,   lt_2Far,    lt_3Near,   lt_3Far,    lt_End
		[td_1        ] = {NONE______, GO_2_NEAR_, GO_2_FAR__, GO_3_NEAR_, GO_3_FAR__, REG_END___},

		[td_2Near    ] = {REG_EXIT__, NONE______, REG_FORK_2, GO_3_NEAR_, ERROR_____, ERROR_____},
		[td_2Far     ] = {ERROR_____, REG_JCT_2_, NONE______, REG_JCT_2_, GO_3_FAR__, ERROR_____},
		[td_3Near    ] = {ERROR_____, ERROR_____, ERROR_____, NONE______, REG_FORK_3, ERROR_____},
		[td_3Far     ] = {ERROR_____, ERROR_____, GO_2_FAR__, REG_JCT_3_, NONE______, ERROR_____},

		[td_2Junction] = {REG_1_____, NONE______, REG_FORK_2, NONE______, REG_FORK_3, ERROR_____},
		[td_3Junction] = {REG_1_____, NONE______, REG_FORK_2, NONE______, REG_FORK_3, ERROR_____},
		[td_2Fork    ] = {REG_1_____, ERROR_____, NONE______, ERROR_____, NONE______, ERROR_____},
		[td_3Fork    ] = {REG_1_____, ERROR_____, NONE______, ERROR_____, NONE______, ERROR_____},
		[td_End      ] = {REG_1_____, ERROR_____, ERROR_____, ERROR_____, ERROR_____, NONE______}
	};

	void NewSection();

	void RegFork2();
	void RegFork3();
	void RegJct2();
	void RegJct3();
	void Reg1();
	void RegEnd();

	void Go2Near();
	void Go2Far();
	void Go3Near();
	void Go3Far();

	void RegExit(); // More difficult...

	void Error(){Reg1();}
	void None(){};
};
