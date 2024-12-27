#include <Arduino.h>
#include "aws_iot.h"
#include "my_wifi.h"
#include "utils.h"
#include "srect_handle.h"

/*
Update ~1000line = 48kb
16:51:35
17:02:34
= 10 phút 59 s
*/
/*=========== MQTT topics ===========*/

#define TOPIC_PUB_DATA "esp8266/pub"   // handleSerialDataAndPublish
#define TOPIC_PUB_OTA "esp8266/status" // messageReceived
#define TOPIC_SUB_OTA "esp8266/ota"    // setup
#define TOPIC_PUB_LOG "esp8266/log"    // log

/*========== Variable ===============*/
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 200;
volatile bool isSrecInProgress = false;
unsigned long lastMillis = 0;
AWSIoT awsIoT;
MYWIFI wifi;

/*========== Prototypes ===============*/
void sendLog(String msg);
void handleSrecTask();
void handleSerialDataAndPublish();
void handleButtonUpdate();
void messageReceived(char *, byte *, unsigned int);

/*========== Function ===============*/

void setup()
{
  Serial.begin(4800);
  /*wait util wifi connected and active access point after 30 seconds wifi not connected*/
  while (!wifi.connect(30000))
  {}
  
  Serial.println("Wifi connected!");
  awsIoT.begin();
  awsIoT.subscribeToTopic(TOPIC_SUB_OTA, messageReceived);
  sendLog("Hello from ESP");

  /*gpio in esp01*/
  pinMode(2, INPUT_PULLUP); // button
  pinMode(0, OUTPUT);       // led
}

void loop()
{
  if (!isSrecInProgress)
  {
    handleSerialDataAndPublish();
    handleButtonUpdate();
    awsIoT.loop();
    wifi.loop();
  }
  else
  {
    handleSrecTask();
  }
}
void handleSrecTask()
{
  digitalWrite(0, HIGH);
  sendSrecData();
  isSrecInProgress = false;
  digitalWrite(0, LOW);
  sendLog("Update firmware finish");
}

void handleSerialDataAndPublish()
{
  if (Serial.available() > 0)
  {
    String str = Serial.readStringUntil('\n');
    char *json = (char *)malloc(200);
    status_t check = convertDataToJson(str, json, 200);
    if (json != nullptr && check == SUCCESS)
    {
      awsIoT.publishMessage(TOPIC_PUB_DATA, json);
    }
    else if (check == NONE)
    {
      awsIoT.publishMessage(TOPIC_PUB_LOG, json); // send log
    }
    free(json);
  }
}
void sendLog(String msg)
{
  char *json = (char *)malloc(200);
  if (json != nullptr && convertDataToJson(msg, json, 200) != ERROR)
  {
    awsIoT.publishMessage(TOPIC_PUB_LOG, json);
  }
  free(json);
}

void handleButtonUpdate()
{
  if (digitalRead(2) == LOW)
  {
    unsigned long currentMillis = millis();
    if (currentMillis - lastButtonPress >= debounceDelay)
    {
      lastButtonPress = currentMillis;
      if (digitalRead(2) == LOW)
      {
        if (!isSrecInProgress)
        {
          isSrecInProgress = true;
        }
        sendLog("Button Clicked");
      }
    }
  }
}

void messageReceived(char *topic, byte *payload, unsigned int length)
{
  // payload to data
  DataSrect_t dt;
  convertJsonToDataSrect(&dt, payload, length);
  // data to srect
  Result_t rs;
  //rs.status = handleSrecData(&dt);
  rs.status=handleSrecAndSend(&dt);
  rs.line = dt.line;
  //Serial.println(rs.line);
  // publish feedback
  char *jsonRs = (char *)malloc(50);
  convertResultToJson(&rs, jsonRs, 50);
  awsIoT.publishMessage(TOPIC_PUB_OTA, jsonRs);
  free(jsonRs);
}