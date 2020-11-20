#include <cstdio>
#include <cstdint>
#include "crc8/crc8.h"

int main()
{
    uint8_t test[] = "Hello!";
    size_t testLen = 6;

    uint8_t crc = Crc8::GetBlockCrc(test, testLen);
    printf("%02X\n", crc);

    Crc8 crcCalculator;
    for (size_t i = 0; i < testLen; i++)
    {
        crcCalculator.PushByte(test[i]);
    }
    crc = crcCalculator.GetCrc();
    printf("%02X\n", crc);

    return 0;
}
