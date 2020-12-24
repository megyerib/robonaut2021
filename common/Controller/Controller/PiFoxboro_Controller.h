#pragma once

#include "Controller.h"

// FOXBORO PI controller parameters
typedef struct
{
	float T_s;
	float T_Plant;
	float K_Plant;
	float u_MIN;
	float u_MAX;
	float T_Cl;
}
PIFB_PARAMS;

class PiFoxboro_Controller : public Controller
{
private:
	PIFB_PARAMS params;

	float K_C;
	float z_d;

	float u_prev  = 0;
	float u_2prev = 0;

public:
	PiFoxboro_Controller(const PIFB_PARAMS& params);

    virtual void Process(float y) override;

    void SetParams(const PIFB_PARAMS& params);
};
