#pragma once

#include "Encoder.h"
#include "Lsm6dso.h"

#define NAVIGATION_SAMPLING_CYCLE   5 /* ms */

#define MILI_DEG_S_TO_RAD_S     (M_PI / 180000.0f)
#define MILI_TO_SI              (1000.0f)

typedef enum
{
    STM = 0,
    GY91
} InertialSensor;

typedef struct  // In NED coordination system.
{
    float n;
    float e;
    float psi;
} Position;

class Navigation
{
private:
    LSM6DSO* inert;
    Encoder& enc;

    Position position;
    float d_s = 0;
    float d_psi = 0;
    float prev_enc = 0;
    float prev_angular = 0;

public:
    static Navigation* GetInstance();
    void Init();

    void Process();
    void SetPosition(Position const pos, float const ds, float const dpsi);
    Position GetPosition();

    uint8_t GetRandomNumber();

private:
    Navigation();

    void Odometry();
    void CalcDeltaOrientation();
    void CalcDeltaDistance();
    float Integral_Trapez(const float a, const float b, const float fa, const float fb);
};
