#ifndef UTILS_H_
#define UTILS_H_
#include <Arduino.h>
#include <ArduinoJson.h>
#include <stdbool.h>
#include "constant.h"

typedef struct Result_t
{
    int line;
    bool status;
} Result_t;
typedef struct DataSrect_t
{
    int line;
    String data;
} DataSrect_t;

status_t convertDataToJson(const String &receive, char *json, size_t maxJsonSize)
{
    status_t status = ERROR;
    // Kiểm tra đầu vào hợp lệ
    if (receive.isEmpty() || json == nullptr || maxJsonSize == 0)
    {
        status = ERROR;
    }
    else
    {
        const size_t capacity = JSON_OBJECT_SIZE(2);
        DynamicJsonDocument doc(capacity);

        // Phân tách chuỗi
        int commaIndex = receive.indexOf(',');
        if (commaIndex == -1)
        {
            doc["device"] = THINGNAME;
            doc["serial"] = receive;
            status = NONE;
        }
        else
        {
            String tempStr = receive.substring(0, commaIndex);
            String humStr = receive.substring(commaIndex + 1);

            // convert string to int
            int temp = tempStr.toInt();
            int hum = humStr.toInt();

            doc["device"] = THINGNAME;
            doc["temperature"] = temp;
            doc["humidity"] = hum;
            status = SUCCESS;
        }


        // Serialize JSON
        size_t jsonLength = serializeJson(doc, json, maxJsonSize - 1);

        if (jsonLength == 0 || jsonLength >= maxJsonSize - 1)
        {
            status = ERROR;
        }

        json[jsonLength] = '\0';
    }

    return status;
}

status_t convertResultToJson(Result_t *rs, char *json, size_t jsonSize)
{
    if (json == nullptr || jsonSize == 0)
    {
        return ERROR;
    }

    DynamicJsonDocument doc(JSON_OBJECT_SIZE(2));
    doc["line"] = rs->line;
    doc["data"] = rs->status ? "ok" : "error";

    // Serialize JSON
    size_t jsonLength = serializeJson(doc, json, jsonSize);
    if (jsonLength == 0 || jsonLength >= jsonSize)
    {
        return ERROR;
    }

    return SUCCESS;
}

status_t convertJsonToDataSrect(DataSrect_t *dt, byte *payload, unsigned int length)
{
    if (dt == nullptr || payload == nullptr)
    {
        return ERROR; // Return error if input pointers are invalid
    }

    // Convert the byte array payload to a string
    String message = "";
    for (unsigned int i = 0; i < length; i++)
    {
        message += (char)payload[i];
    }

    // Parse the JSON document
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, message);

    if (error)
    {
        return ERROR; // Return error if JSON parsing fails
    }
    dt->line = doc["line"] | -1;
    dt->data = doc["data"] | "";

    return SUCCESS; // Return success
}

#endif /*UTILS_H_*/