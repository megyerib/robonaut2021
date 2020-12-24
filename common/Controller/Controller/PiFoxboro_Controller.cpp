#include "PiFoxboro_Controller.h"
#include <math.h>

#define SATURATE(x, min, max)  ((x) = (x) > (max) ? (max) : ((x) < (min) ? (min) : (x)))

PiFoxboro_Controller::PiFoxboro_Controller(const PIFB_PARAMS& params)
{
	SetParams(params);
}

void PiFoxboro_Controller::Process(float y)
{
	float e = ref - y;
	float u_1 = K_C * e;
	float u_2 = z_d * u_2prev + (1.0 - z_d) * u_prev;
	float u = u_1 + u_2;
	SATURATE(u, params.u_MIN, params.u_MAX);

	u_prev  = u;
	u_2prev = u_2;

	cout = u;
}

void PiFoxboro_Controller::SetParams(const PIFB_PARAMS& p)
{
	this->params = p;

	float p_d = exp(-params.T_s/params.T_Plant);
	float K_d = (params.K_Plant * (1 - exp(-params.T_s/params.T_Plant)));

	K_C = (1.0 / K_d * (1.0 - exp(-params.T_s/params.T_Cl)));
	z_d = p_d;
}
