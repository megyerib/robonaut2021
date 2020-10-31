#include <cstdio>
#include <cstring>
#include "SerialMessages.h"
#include "crc8.h"
#include "EscapeEncoder.h"
#include <cmath>

#define FILE_NAME  "test.txt"
#define BUF_LEN    (30u)      /* Length of every buffer in this example */

// Print a Serial Message (SM) into the FILE_NAME file
// Also adds CRC
void printSM(void* msg, size_t len);

// Print a string into the FILE_NAME file as a text type message
// Also adds CRC
void printText(char* str);

// Generate a test file with dummy data (a second long sine period with 1 ms resolution) + some text
void generateTestFile();

// Reads the test files, decodes the lines and calculates CRC
void processTestFile();

int main()
{
    generateTestFile();
    processTestFile();

    return 0;
}

void generateTestFile()
{
    // Clear file
    FILE *f = fopen(FILE_NAME, "w");

    if (f == NULL)
    {
        return;
    }

    // Generate dummy messages (0x01)
    SM_DUMMY dummy;

    dummy.id = smDummy;

    for (int i = 0; i < 1000; i++)
    {
        dummy.timestamp = i * 10;
        dummy.value = 1000 * sinf(i * 6.2832 / 1000.0);
        printSM(&dummy, sizeof(dummy));
    }

    // Generate text messages (0x00)
    printText((char*)"siiinus");
    printText((char*)"uuund");
    printText((char*)"cooosiiinus");
    printText((char*)"jaaa");
}

void processTestFile()
{
    // Open file
    FILE *f = fopen(FILE_NAME, "rb");

    if (f == NULL)
    {
        return;
    }

    size_t lineLen = 0;
    BinaryEncoder* enc = new EscapeEncoder;
    uint8_t line[BUF_LEN];

    while (1)
    {
        size_t cnt;
        cnt = fread(&line[lineLen], 1, 1, f);

        if (cnt != 0) // The data was read, EOF hasn't been reached
        {
            if (line[lineLen] != '\n')
            {
                lineLen++;
            }
            else // Line end reached
            {
                uint8_t decBuf[BUF_LEN];
                size_t decBufLen;

                // Decode line
                enc->Decode(line, lineLen, decBuf, decBufLen);

                // Process data (Check CRC for example)
                if (Crc8::CheckBlockCrc(decBuf, decBufLen))
                {
                    puts("CRC ok");
                }
                else
                {
                    puts("CRC error");
                }

                // Reset line length, proceed to the next one
                lineLen = 0;
            }
        }
        else // EOF has been reached
        {
            break;
        }
    }

    // Close file
    fclose(f);
}

void printSM(void* msg, size_t len)
{
    // 1 Add CRC
    uint8_t srcBuf[BUF_LEN];
    size_t srcBufLen;

    memcpy(srcBuf, msg, len);
    srcBuf[len] = Crc8::GetBlockCrc((uint8_t*)msg, len);
    srcBufLen = len + 1;

    // 2 Encode
    uint8_t encBuf[BUF_LEN];
    size_t encBufLen;
    BinaryEncoder* enc = new EscapeEncoder;

    enc->Encode(srcBuf, srcBufLen, encBuf, encBufLen);

    // 3 Add '\n'
    encBuf[encBufLen] = '\n';
    encBufLen++;

    // 4 Print to file
    FILE *f = fopen(FILE_NAME, "ab"); // a: append, b: binary
    // Without binary LF will be converted into CR LF

    if (f != NULL)
    {
        fwrite(encBuf, 1, encBufLen, f);
        fclose(f);
    }
}

void printText(char* str)
{
    // Abb the text message code (0x00) to the start of the buffer
    uint8_t buf[BUF_LEN];
    SM_TEXT* message = (SM_TEXT*) buf;
    message->id = smText;
    size_t len = strlen(str);

    memcpy(message->text, str, len);

    printSM(message, len + 1);
}
