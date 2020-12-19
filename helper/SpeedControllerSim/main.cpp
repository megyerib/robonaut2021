#include <stdio.h>
#include <math.h>
#include <locale.h>

#define SATURATE(x, min, max)  ((x) = (x) > (max) ? (max) : ((x) < (min) ? (min) : (x)))

float Process(float u);
float Controller(float r, float y);
float Random(float min, float max);

#define T_s 0.01f

#define t_1   5.0
#define t_2  10.0

int main()
{
    setlocale(LC_NUMERIC, "French_Canada.1252");

    FILE* f = fopen("data.csv", "w");

    fprintf(f, "t;r;u;y\n");

    float r, u, y;

    for (float t = -1.0; t <= 0.0; t += T_s)
    {
        r = 0.0f;
        u = 0.0f;
        y = 0.0f;

        fprintf(f, "%f;%f;%f;%f\n", t, r, u, y);
    }

    for (float t = T_s; t <= t_1; t += T_s)
    {
        r = 100.0f;
        u = Controller(r, y);
        y = Process(u);

        fprintf(f, "%f;%f;%f;%f\n", t, r, u, y);
    }

    for (float t = t_1 + T_s; t <= t_2; t += T_s)
    {
        r = 0.0f;
        u = Controller(r, y);
        y = Process(u);

        fprintf(f, "%f;%f;%f;%f\n", t, r, u, y);
    }

    fclose(f);

    return 0;
}

#define K   0.9f
#define T   1.0f
#define p_d exp(-T_s/T)
#define K_d (K * (1 - exp(-T_s/T)))
#define T_Cl 0.5f
#define K_C (1.0 / K_d * (1.0 - exp(-T_s/T_Cl)))
#define z_d p_d
#define MAX_u 300.0f

float Controller(float r, float y)
{
    static float u_prev;
    static float u_2prev;

    float e = r - y;
    float u_1 = K_C * e;
    float u_2 = z_d * u_2prev + (1.0 - z_d) * u_prev;
    float u = u_1 + u_2;
    SATURATE(u, -150, 150);

    u_prev  = u;
    u_2prev = u_2;

    return u;
}

float Process(float input)
{
    static float state = 0;

    SATURATE(input, -150, 150);

    float e = input - state;

    state += e * (1 - exp(-T_s/T));

    return K * state + Random(-5, 5); // y
}

float Random(float min, float max)
{
    float range = max - min;

    return min + (float)rand()/(float)(RAND_MAX/range);
}
