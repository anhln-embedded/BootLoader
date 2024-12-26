#ifndef SRECT_HANDLE_H
#define SRECT_HANDLE_H

#include <LittleFS.h>  
#include "utils.h"
#define MAX_SREC_SIZE 102400  // 100KB
#define START_SIGNAL "hello"
#define FINISH_SIGNAL "bye"
#define FILE_PATH "/app.srec"
File srecFile; 


inline bool isHexDigit(char c) {
    return (('0' <= c && c <= '9') || ('A' <= c && c <= 'F') || ('a' <= c && c <= 'f'));
}

bool isValidSrecRecord(const String& record) {
    return true;
}



bool initSrecFile() {
    if (!LittleFS.begin()) {       
        return false;
    }    
    // Delete existing file if it exists to start fresh
    if (LittleFS.exists(FILE_PATH)) { 
        LittleFS.remove(FILE_PATH); 
    }
    // Open file in write mode to create a new file
    srecFile = LittleFS.open(FILE_PATH, "w"); 

    if (!srecFile) {
        return false;
    }
    return true;
}

bool writeSrecData(const String& data) {
    // Validate the SREC record format
    if (!isValidSrecRecord(data)) {
        Serial.println("Invalid format, do not write");
        return false;  // Invalid format, do not write
    }
    if (srecFile.size() + data.length() > MAX_SREC_SIZE) {
        Serial.println("Not enough space in Flash to write data");
        return false;  // Not enough space in Flash to write data.
    }

    // Write data to file
    srecFile.print(data);
    Serial.println("Data written to SREC file.");
    return true;
}

void closeSrecFile() {
    if (srecFile) {
        srecFile.close();
    }
}

size_t getSrecFileSize() {
    return srecFile.size();
}

// Handle data when received and write to SREC file
bool handleSrecData(const DataSrect_t * dt) {
    if (dt->line == -1) {
        if (dt->data == START_SIGNAL) {  // Reinitialize the file (removing old data)
            return initSrecFile();
        }else if (dt->data == FINISH_SIGNAL) {  // Received byte indicating end of file. Stopping data write.
            closeSrecFile();
            return true;
        }         
    } 
    // Write data to file when line >= 0
    return writeSrecData(dt->data);
}


#endif /* SRECT_HANDLE_H */
