#include <cstdio>
#include <cstring>

#include "EscapeEncoder.h"

void hexDump(const uint8_t* arr, size_t len);

int main()
{
    BinaryEncoder* encoder = new EscapeEncoder;

    uint8_t testString[] = "Asd\nLol";
    size_t testStringSize = strlen((char*)testString);

    uint8_t encoded[20];
    size_t encodedSize;
    uint8_t decoded[20];
    size_t decodedSize;

    encoder->Encode(testString, testStringSize, encoded, encodedSize, 20);
    encoder->Decode(encoded, encodedSize, decoded, decodedSize, 20);

    hexDump(testString, testStringSize);
    hexDump(encoded, encodedSize);
    hexDump(decoded, decodedSize);

    return 0;
}

void hexDump(const uint8_t* arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        printf("%02X ", arr[i]);
    }

    printf("\n");
}
