#include <cstdio>
#include <cstring>
#include "SerialMessages.h"
#include "crc8.h"
#include "EscapeEncoder.h"
#include <cmath>
#include "MainLsMsg.h"

#define FILE_NAME  "test.txt"
#define HEX_FILE_NAME  "test_hex.txt"
#define BUF_LEN    (100u)      /* Length of every buffer in this example */

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

// Reads line sensor data raw output from file and convert them to csv
void processLineFile(char* src, char* dst);

void dumpBuffer(uint8_t buf[], size_t len);

int main()
{
    // Random line sensor command generation
    /*M2L_CFG cfg_msg;

    cfg_msg.LedEn = 0;
    cfg_msg.LineDataEn = 0;
    cfg_msg.MeasEn = 1;
    cfg_msg.SensorDataEn = 0;

    printSM_hex(stdout, &cfg_msg, sizeof(cfg_msg));

    cfg_msg.LedEn = 1;
    printSM_hex(stdout, &cfg_msg, sizeof(cfg_msg));

    cfg_msg.SensorDataEn = 1;
    printSM_hex(stdout, &cfg_msg, sizeof(cfg_msg));*/
    processLineFile("floor-black.txt", "floor-black.csv");

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
                enc->Decode(line, lineLen, decBuf, decBufLen, BUF_LEN);

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

void processLineFile(char* src, char* dst)
{
    // Open file
    FILE *f      = fopen(src, "rb");
    FILE *csv = fopen(dst, "w");
    size_t lineNum = 1;

    if (f == NULL || csv == NULL)
    {
        return;
    }

    size_t lineLen = 0;
    EscapeEncoder enc;
    uint8_t line[BUF_LEN];
    bool discardLine = false;

    while (1)
    {
        size_t cnt;
        cnt = fread(&line[lineLen], 1, 1, f);

        if (cnt != 0) // The data was read, EOF hasn't been reached
        {
            if (line[lineLen] != '\n')
            {
                if (lineLen < BUF_LEN)
                {
                    lineLen++;
                }
                else
                {
                    discardLine = true;
                }
            }
            else // Line end reached
            {
                uint8_t decBuf[BUF_LEN];
                size_t decBufLen;
                L2M_SENSOR_DATA* msg = (L2M_SENSOR_DATA*) decBuf;

                if (lineLen > 0)
                {
                    lineLen--;
                }

                // Decode line
                if (!discardLine)
                {
                    enc.Decode(line, lineLen, decBuf, decBufLen, BUF_LEN);
                }

                // Process data
                if (discardLine)
                {
                    printf("Line too long! (>%llu; line %llu)\n", lineLen, lineNum);
                }
                else if (msg->ID != l2mSensorData || decBufLen != 66)
                {
                    printf("Invalid line (%llu; line %llu): ", decBufLen, lineNum);
                    dumpBuffer(decBuf, decBufLen);
                }
                else
                {
                    fprintf(csv, "%d", msg->data[0]);

                    for (int i = 1; i < msg->size; i++)
                    {
                        fprintf(csv, ";%d", msg->data[i]);
                    }

                    fprintf(csv, "\n");
                }

                // Reset line length, proceed to the next one
                lineLen = 0;
                discardLine = false;
                lineNum++;
            }
        }
        else // EOF has been reached
        {
            break;
        }
    }

    // Close file
    fclose(f);
    fclose(csv);
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

    enc->Encode(srcBuf, srcBufLen, encBuf, encBufLen, BUF_LEN);

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

    enc->Encode(srcBuf, srcBufLen, encBuf, encBufLen, BUF_LEN);

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

void dumpBuffer(uint8_t buf[], size_t len)
{
    if (len > 0)
    {
        printf("%d", buf[0]);

        for (size_t i = 0; i < len; i++)
        {
            printf(";%d", buf[i]);
        }

        printf("\n");
    }
    else
    {
        printf("Empty buffer\n");
    }
}
