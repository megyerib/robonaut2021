#pragma once

#include <stdint.h>
#include "RemoteTimer.h"

typedef enum
{
	chSteering = 0,
	chThrottle,

	chNum
}
RemoteChannel;

// State of the mode switching button (CH3)
// Default: Mode1
typedef enum
{
	RemMode1,
	RemMode2
}
RemoteMode;

typedef struct
{
	uint32_t min;
	uint32_t mid;
	uint32_t max;
}
RemoteCal;

class Remote
{
public:
	static Remote& GetInstance();
	float GetValue(RemoteChannel ch); /* [-1; +1] */
	RemoteMode GetMode();

	bool CalibrationStart(RemoteChannel ch);
	bool IsCalibrationInProgress(RemoteChannel ch);
	void GetCalibrationValues(RemoteChannel ch, RemoteCal& cal);
	void SetCalibrationValues(RemoteChannel ch, RemoteCal& cal);

	void CalibrationProcess();

private:
	RemoteCal calData[chNum]  = {0};
	bool calInProgress[chNum] = {false};
	bool calStarted[chNum] = {false};
	RemoteHw* remoteHw;

	Remote();
};
