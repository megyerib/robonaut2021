#pragma once

#define TRACTION_CONTROL_PERIOD   10 /* ms */

typedef enum
{
    tmDutyCycle = 0
}
TractionMode;

class Traction
{
public:
	static Traction* GetInstance();
	void Process();

	void SetDutyCycle(float d)   {dutyCycle = d;}  // [-1;+1]
	void SetMode(TractionMode m) {mode = m;}

private:
	TractionMode mode;
	float dutyCycle;

	Traction();

	void SendDutyCycle(float d); // -1 <= d <= 1
	int sprintint(char* dst, int n);
};
