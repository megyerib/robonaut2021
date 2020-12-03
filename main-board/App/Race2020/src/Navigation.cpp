#include "Navigation.h"

Navigation* Navigation::GetInstance()
{
    static Navigation instance;
    return &instance;
}

void Navigation::Init()
{
    inert->Init();

    position.n   = 0.0f;
    position.e   = 0.0f;
    position.psi = 0.0f;
    d_s          = 0.0f;
    d_psi        = 0.0f;
    prev_enc     = enc.GetDistance();
    prev_angular = inert->GetAngular_mdps(Axis::yaw);
}

void Navigation::Process()
{
    inert->Process();

    Odometry();
}


void Navigation::SetPosition(Position const pos, float const ds, float const dpsi)
{
    position     = pos;
    d_s          = ds;
    prev_enc     = enc.GetDistance() - d_s;
    d_psi        = dpsi;
    prev_angular = inert->GetAngular_mdps(Axis::yaw) - d_psi;
}

Position Navigation::GetPosition()
{
    return position;
}

uint8_t Navigation::GetRandomNumber()
{
    return inert->GetRandomNumber();
}

Navigation::Navigation() : enc(Encoder::GetInstance())
{
    inert = new LSM6DSO();
}

void Navigation::Odometry()
{
    CalcDeltaOrientation();
    CalcDeltaDistance();

    // psi_k = psi_k-1 + d_psi
    position.psi = position.psi + d_psi;

    // n_k = n_k-1 + d_s * cos(psi_k)
    position.n = position.n + d_s * cosf(position.psi);

    // e_k = e_k-1 + d_s * sin(psi_k)
    position.e = position.e + d_s * sinf(position.psi);
}

void Navigation::CalcDeltaOrientation()
{
    // TODO
    float angular_yaw;

    angular_yaw = inert->GetAngular_mdps(Axis::yaw) * MILI_DEG_S_TO_RAD_S;

    d_psi = Integral_Trapez(0.0f, NAVIGATION_SAMPLING_CYCLE / MILI_TO_SI, prev_angular, angular_yaw);

    prev_angular = angular_yaw;
}

void Navigation::CalcDeltaDistance()
{
    float act_enc = enc.GetDistance();

    d_s = act_enc - prev_enc;

    prev_enc = act_enc;
}

float Navigation::Integral_Trapez (const float a, const float b, const float fa, const float fb)
{
    float integral;

    integral = (b - a) * (fb + fa) / 2;

    return integral;
}
