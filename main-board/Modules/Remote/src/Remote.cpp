#include "Remote.h"

#define MODE_THRESHOLD  1100 /* us */
#define CALIB_THRESHOLD 50 /* us */
#define ABS(x) (((x) < 0) ? -(x) : (x))

const RemoteHwChannel chTable[CH_Num] =
{
	[SteeringCh] = RemCh1,
	[ThrottleCh] = RemCh2
};

Remote::Remote()
{
	remoteHw = RemoteHw::GetInstance();

	calData[SteeringCh].min = 995;
	calData[SteeringCh].mid = 1509;
	calData[SteeringCh].max = 2110;

	calData[ThrottleCh].min = 1001;
	calData[ThrottleCh].mid = 1503;
	calData[ThrottleCh].max = 2080;
}

Remote* Remote::GetInstance()
{
	static Remote instance;
	return &instance;
}

float Remote::GetValue(RemoteChannel ch)
{
	float ret = 0;
	uint16_t pulse = remoteHw->GetPulseWidth(chTable[ch]);
	RemoteCal& cal = calData[ch];

	if (pulse == 0) // First measurment
	{
		return 0;
	}

	if (pulse > cal.mid)
	{
		if ((cal.max - cal.mid) != 0)
		{
			int16_t diff       = pulse - cal.mid;
			int16_t full_scale = cal.max - cal.mid;

			ret = (float)diff/full_scale;
		}

		if (ret > 1.0)
		{
			ret = 1.0;
		}
	}
	else
	{
		if ((cal.mid - cal.min) != 0)
		{
			int16_t diff       = pulse - cal.mid;
			int16_t full_scale = cal.mid - cal.min;

			ret = (float)diff/full_scale;
		}

		if (ret < -1.0)
		{
			ret = -1.0;
		}
	}

	return ret;
}

RemoteMode Remote::GetMode()
{
	uint16_t pulse = remoteHw->GetPulseWidth(RemCh3);

	if (pulse < MODE_THRESHOLD)
	{
		return RemMode1;
	}
	else
	{
		return RemMode2;
	}
}

bool Remote::CalibrationStart(RemoteChannel ch)
{
	if (remoteHw->GetPulseWidth(chTable[ch]) > 0)
	{
		calInProgress[ch] = true;
		calStarted[ch] = true;
		return true;
	}
	else
	{
		return false;
	}
}

bool Remote::IsCalibrationInProgress(RemoteChannel ch)
{
	return calInProgress[ch];
}

void Remote::GetCalibrationValues(RemoteChannel ch, RemoteCal& cal)
{
	cal = calData[ch];
}

void Remote::SetCalibrationValues(RemoteChannel ch, RemoteCal& cal)
{
	calData[ch] = cal;
}

void Remote::CalibrationProcess()
{
	static RemoteCal tmpData[CH_Num];

	for (int ch = 0; ch < CH_Num; ch++)
	{
		if (calInProgress[ch])
		{
			uint16_t pulse = remoteHw->GetPulseWidth(chTable[ch]);

			if (calStarted[ch])
			{
				tmpData[ch] = {pulse, pulse, pulse};

				calStarted[ch] = false;
			}
			else
			{
				if (pulse > (tmpData[ch].mid + CALIB_THRESHOLD) && pulse > tmpData[ch].max)
				{
					tmpData[ch].max = pulse;
				}

				if (pulse < (tmpData[ch].mid - CALIB_THRESHOLD) && pulse < tmpData[ch].min)
				{
					tmpData[ch].min = pulse;
				}

				if (tmpData[ch].min < tmpData[ch].mid && tmpData[ch].max > tmpData[ch].mid && ABS(tmpData[ch].mid - pulse) < CALIB_THRESHOLD)
				{
					calData[ch] = tmpData[ch];
					calInProgress[ch] = false;
				}
			}
		}
	}
}
