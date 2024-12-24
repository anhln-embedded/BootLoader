#ifndef UTILS_H_
#define UTILS_H_
#include <Arduino.h>
#include <ArduinoJson.h>
#include "constant.h"

status_t convertDataToJson(const String& receive, char* json, size_t maxJsonSize) {
    // Kiểm tra đầu vào hợp lệ
    if (receive.isEmpty() || json == nullptr || maxJsonSize == 0) {
        return ERROR;
    }

    const size_t capacity = JSON_OBJECT_SIZE(2); 
    DynamicJsonDocument doc(capacity);

    // Phân tách chuỗi
    int commaIndex = receive.indexOf(',');
    if (commaIndex == -1) {
        return ERROR;
    }

    String tempStr = receive.substring(0, commaIndex);
    String humStr = receive.substring(commaIndex + 1);

    // convert string to int
    int temp = tempStr.toInt();
    int hum = humStr.toInt();

    doc["device"] = THINGNAME;
    doc["temperature"] = temp;
    doc["humidity"] = hum;

    // Serialize JSON
    size_t jsonLength = serializeJson(doc, json, maxJsonSize - 1);

    if (jsonLength == 0 || jsonLength >= maxJsonSize - 1) {
        return ERROR;
    }

    json[jsonLength] = '\0'; 
    return SUCCESS;
}



#endif /*UTILS_H_*/