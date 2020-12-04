#include <cstdio>
#include <cstring>
#include "SerialMessages.h"
#include "crc8.h"
#include "EscapeEncoder.h"
#include <cmath>

#define FILE_NAME  "test.txt"
#define HEX_FILE_NAME  "test_hex.txt"
#define BUF_LEN    (30u)      /* Length of every buffer in this example */

// Print a Serial Message (SM) into the FILE_NAME file
// Also adds CRC
void printSM(FILE* f, void* msg, size_t len);
void printSM_hex(FILE* f, void* msg, size_t len);

// Print a string into the FILE_NAME file as a text type message
// Also adds CRC
void printText(FILE* f, char* str);

// Generate a test file with dummy data (a second long sine period with 1 ms resolution) + some text
void generateTestFile();

// Reads the test files, decodes the lines and calculates CRC
void processTestFile();

int main()
{
    uint8_t reset[3] = {0};
    printSM_hex(stdout, reset, 1);

    uint8_t setNum[3] = {1, 20};
    printSM_hex(stdout, setNum, 2);

    //generateTestFile();
    //processTestFile();

    return 0;
}

void generateTestFile()
{
    // Clear files
    FILE *f   = fopen(FILE_NAME,     "w"); // Append binary
    FILE *f_h = fopen(HEX_FILE_NAME, "w"); // Without binary LF will be converted into CR LF
    fclose(f);
    fclose(f_h);

    // Open files
    f   = fopen(FILE_NAME,     "ab"); // Append binary
    f_h = fopen(HEX_FILE_NAME, "ab"); // Without binary LF will be converted into CR LF

    if (f == NULL || f_h == NULL)
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
        printSM(f, &dummy, sizeof(dummy));
        printSM_hex(f_h, &dummy, sizeof(dummy));
    }

    // Generate text messages (0x00)
    printText(f, (char*)"siiinus");
    printText(f, (char*)"uuund");
    printText(f, (char*)"cooosiiinus");
    printText(f, (char*)"jaaa");

    fclose(f);
    fclose(f_h);
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

void printSM(FILE* f, void* msg, size_t len)
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
    if (f != NULL)
    {
        fwrite(encBuf, 1, encBufLen, f);
    }
}

void printSM_hex(FILE* f, void* msg, size_t len)
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
    if (f != NULL)
    {
        //fwrite(encBuf, 1, encBufLen, f);
        fprintf(f, "0x");

        for (unsigned int i = 0; i < encBufLen; i++)
        {
            fprintf(f, "%02X", encBuf[i]);
        }

        fprintf(f, "\n");
    }
}

void printText(FILE* f, char* str)
{
    // Abb the text message code (0x00) to the start of the buffer
    uint8_t buf[BUF_LEN];
    SM_TEXT* message = (SM_TEXT*) buf;
    message->id = smText;
    size_t len = strlen(str);

    memcpy(message->text, str, len);

    printSM(f, message, len + 1);
}
