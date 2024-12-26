#include <Arduino.h>
#include "aws_iot.h"
#include "my_wifi.h"
#include "utils.h"
#include "srect_handle.h"
/*=========== MQTT topics ===========*/

#define TOPIC_PUB_DATA "esp8266/pub"   // handleSerialDataAndPublish
#define TOPIC_PUB_OTA "esp8266/status" // messageReceived
#define TOPIC_SUB_OTA "esp8266/ota"    // setup

/*========== Variable ===============*/

unsigned long lastMillis = 0;
AWSIoT awsIoT;
MYWIFI wifi;

/*========== Prototypes ===============*/

void handleSerialDataAndPublish();
void handleButtonUpdate();
void messageReceived(char *, byte *, unsigned int);

/*========== Function ===============*/

void setup()
{
  Serial.begin(4800);
  Serial.println("Start");

  /*wait util wifi connected and active access point after 30 seconds wifi not connected*/
  while (!wifi.connect(30000))
    ;
  Serial.println("Wifi connected!");
  awsIoT.begin();
  awsIoT.subscribeToTopic(TOPIC_SUB_OTA, messageReceived); // listen ota trigger

  /*gpio in esp01*/
  pinMode(2, INPUT_PULLUP); // button
  pinMode(0, OUTPUT);       // led
}

void loop()
{
  handleSerialDataAndPublish();
  handleButtonUpdate();
  awsIoT.loop();
  wifi.loop();
}

void handleSerialDataAndPublish()
{
  if (Serial.available() > 0)
  {
    String str = Serial.readStringUntil('\n');
    char *json = (char *)malloc(200);
    if (json != nullptr && convertDataToJson(str, json, 200) == SUCCESS)
    {
      awsIoT.publishMessage(TOPIC_PUB_DATA, json);
      free(json);
    }
  }
}
void handleButtonUpdate()
{
  if (digitalRead(2) == 0)
  {
    delay(200);
    if (digitalRead(2) == 0)
    {
      digitalWrite(0, HIGH);
      Serial.print("ffff");
      delay(500);
      digitalWrite(0, LOW);
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
  rs.status = handleSrecData(&dt);
  rs.line = dt.line;
  Serial.println(rs.line);
  // publish feedback
  char *jsonRs = (char *)malloc(50);
  convertResultToJson(&rs, jsonRs, 50);
  awsIoT.publishMessage(TOPIC_PUB_OTA, jsonRs);
  free(jsonRs);

}