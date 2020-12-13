#include "LineFilter.h"
#include <cstdio>

bool LineInputEquals(LineInput& li1, LineInput& li2);
void test(LineInput meas[5], size_t index, LineInput expRes);

int main()
{
    // Test case 1
    LineInput tc1meas[5] =
    {
        {{0,0,0}, 0, 0},
        {{0,0,0}, 0, 0},
        {{0,0,0}, 0, 0},
        {{0,0,0}, 0, 0},
        {{0,0,0}, 0, 0}
    };

    size_t tc1index = 3;

    LineInput tc1ExpRes = {{0,0,0}, 0, 0};

    test(tc1meas, tc1index, tc1ExpRes);

    // Test case 2
    LineInput tc2meas[5] =
    {
        {{     0,-14000, 15000}, 3, 0},
        {{   500, 15500,     0}, 2, 0},
        {{  -200, 15300,     0}, 2, 0},
        {{ 16000,     0,     0}, 1, 0},
        {{-20000,     0, 14900}, 3, 0}
    };

    size_t tc2index = 4;

    LineInput tc2ExpRes = {{0,14900,0}, 2, 0};

    test(tc2meas, tc2index, tc2ExpRes);

    // Test case 3
    LineInput tc3meas[5] =
    {
        {{  -200,     0,     0}, 1, 0},
        {{     0, 16000,     0}, 2, 0},
        {{ 15000, 25000,     0}, 2, 0},
        {{-14000,     0, 15000}, 3, 0},
        {{   500,     0,     0}, 1, 0}
    };

    size_t tc3index = 2;

    LineInput tc3ExpRes = {{0,15000,0}, 2, 0};

    test(tc3meas, tc3index, tc3ExpRes);

    // Test case 4
    LineInput tc4meas[5] =
    {
        {{  2000, 10000,  8000}, 3, 0},
        {{ -4000,  2000,  6000}, 3, 0},
        {{ -4000,  6000, 10000}, 3, 0},
        {{ -4000,  6000,  8000}, 3, 0},
        {{  2000, 10000,  8000}, 3, 0}
    };

    size_t tc4index = 0;

    LineInput tc4ExpRes = {{2000,6000,8000}, 3, 0};

    test(tc4meas, tc4index, tc4ExpRes);
}

// Test helper functions

bool LineInputEquals(LineInput& li1, LineInput& li2)
{
    if (li1.cnt > 3 || li2.cnt > 3)
        return false;

    if (li1.cnt != li2.cnt)
        return false;

    for (int i = 0; i <= li1.cnt; i++)
    {
        if (li1.lines[i] != li2.lines[i])
            return false;
    }

    return true;
}

void test(LineInput meas[5], size_t index, LineInput expRes)
{
    LineInput res = FilterLines(meas, index);

    if (LineInputEquals(res, expRes))
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}
