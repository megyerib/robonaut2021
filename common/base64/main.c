#include <stdio.h>
#include "base64.h"
#include <string.h>

#define STR_MAX_LEN 30

typedef struct
{
    char decoded[STR_MAX_LEN];
    char encoded[STR_MAX_LEN];
}
TEST_CASE;

static TEST_CASE TestCases[] =
{
    {"",             ""},
    {"A",            "QQ"},
    {"AS",           "QVM"},
    {"ASD",          "QVNE"},
    {"ASDL",         "QVNETA"},
    {"ASDLO",        "QVNETE8"},
    {"ASDLOL",       "QVNETE9M"},
    {"Hello World!", "SGVsbG8gV29ybGQh"}
};

int main()
{
    printf("ENCODING --------------\n"); // ---------------------------------------------------

    for (size_t i = 0; i < (sizeof(TestCases)/sizeof(TEST_CASE)); i++)
    {
        char* decoded = TestCases[i].decoded;
        char encoded[30] = {0};
        size_t encoded_len = 0;

        base64_encode((uint8_t*)decoded, (uint8_t*)encoded, strlen(decoded), &encoded_len);

        if (strncmp(encoded, TestCases[i].encoded, STR_MAX_LEN) == 0 &&
            encoded_len == strlen(encoded) &&
            encoded_len == strlen(TestCases[i].encoded))
        {
            printf("Test %d passed\n", (int)i);
        }
        else
        {
            printf("Test %d FAILED !\n", (int)i);
        }
    }

    printf("DECODING --------------\n"); // ---------------------------------------------------

    for (size_t i = 0; i < (sizeof(TestCases)/sizeof(TEST_CASE)); i++)
    {
        char* encoded = TestCases[i].encoded;
        char decoded[30] = {0};
        size_t decoded_len = 0;

        base64_decode((uint8_t*)encoded, (uint8_t*)decoded, strlen(encoded), &decoded_len);

        if (strncmp(decoded, TestCases[i].decoded, STR_MAX_LEN) == 0 &&
            decoded_len == strlen(decoded) &&
            decoded_len == strlen(TestCases[i].decoded))
        {
            printf("Test %d passed\n", (int)i);
        }
        else
        {
            printf("Test %d FAILED !\n", (int)i);
        }
    }

    return 0;
}
