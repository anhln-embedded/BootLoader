#ifndef SRECT_HANDLE_H
#define SRECT_HANDLE_H

#include <LittleFS.h>
#include "utils.h"
#include "parsing_srec.h"
#define MAX_SREC_SIZE 102400 // 100KB
#define START_SIGNAL "hello"
#define FINISH_SIGNAL "bye"
#define FILE_PATH "/app.srec"
File srecFile;
bool initFile = false;

bool isSrecFileEmpty()
{
    if (!LittleFS.exists(FILE_PATH))
    {
        return true;
    }
    File srecFile = LittleFS.open(FILE_PATH, "r");
    if (!srecFile)
    {
        return true;
    }

    size_t fileSize = srecFile.size();
    srecFile.close();
    return (fileSize == 0);
}


bool isValidSrecRecord(const String &data)
{
    // char* record = new char[data.length() + 1]; 
    // data.toCharArray(record, data.length() + 1); 

    // SrecLine_t parsedData;
    // ErrorCode_t result = SrecReadLine(record, &parsedData);
    // delete[] record;

    // if (result == SREC_ERROR_NOERR)
    // {
    //     return true;  
    // }
    // else
    // {
    //     return false; 
    // }
    return true;
}


bool initSrecFile()
{
    if (!LittleFS.begin())
    {
        return false;
    }
    // Delete existing file if it exists to start fresh
    if (LittleFS.exists(FILE_PATH))
    {
        LittleFS.remove(FILE_PATH);
    }
    // Open file in write mode to create a new file
    srecFile = LittleFS.open(FILE_PATH, "w");

    if (!srecFile)
    {
        return false;
    }
    initFile = true;
    return true;
}

bool writeSrecData(const String &data)
{
    if (!isValidSrecRecord(data))
    {
        return false; 
    }
    if (srecFile.size() + data.length() > MAX_SREC_SIZE)
    {
        // Serial.println("Not enough space in Flash to write data");
        return false; // Not enough space in Flash to write data.
    }

    // Write data to file
    srecFile.print(data);
    srecFile.print("\n");
    srecFile.flush();
    return true;
}

void closeSrecFile()
{
    if (srecFile)
    {
        initFile = false;
        srecFile.close();
    }
}

size_t getSrecFileSize()
{
    return srecFile.size();
}

void sendSrecData()
{
    if (!LittleFS.exists(FILE_PATH))
    {
        // Serial.println("SREC file does not exist.");
        return;
    }

    // Open the file in read mode
    File srecFile = LittleFS.open(FILE_PATH, "r");
    if (!srecFile)
    {
        // Serial.println("Failed to open SREC file for reading.");
        return;
    }
    Serial.print("ffff");
    delay(100);
    Serial.print('y');
    noInterrupts();
    char buffer[128];
    while (srecFile.available())
    {
        size_t len = srecFile.readBytesUntil('\n', buffer, sizeof(buffer) - 1);
        buffer[len] = '\0';
        Serial.println(buffer);
        delay(50);
        yield();
    }
    interrupts();
    digitalWrite(0, LOW);
    srecFile.close();
}
// Handle data when received and write to SREC file
bool handleSrecData(const DataSrect_t *dt)
{
    bool status = false;
    if (dt->line == -1)
    {
        if (dt->data == START_SIGNAL)
        { // Reinitialize the file (removing old data)
            if (initFile)
            {
                closeSrecFile();
            }
            status = initSrecFile();
            digitalWrite(0, HIGH);
        }
        else if (dt->data == FINISH_SIGNAL)
        { // Received byte indicating end of file. Stopping data write.
            closeSrecFile();
            // sendSrecData();
            digitalWrite(0, LOW);
            status = true;
        }
    }
    else if (initFile)
    {
        // Write data to file when line >= 0
        status = writeSrecData(dt->data);
    }

    return status;
}
bool handleSrecAndSend(const DataSrect_t *dt)
{
    if (dt->line < 0)
    {
        if (dt->data == START_SIGNAL)
        { 
            Serial.print("ffff");
            delay(500);
            Serial.print('y');
            digitalWrite(0, HIGH);
        }
        else if (dt->data == FINISH_SIGNAL)
        { 
            digitalWrite(0, LOW);
        }
    }
    else if (isValidSrecRecord(dt->data))
    {
        Serial.println(dt->data);
    }else{
        return false;
    }

    return true;
}
#endif /* SRECT_HANDLE_H */
