#include <Arduino.h>
#include "aws_iot.h"
#include "my_wifi.h"
#include "utils.h"
unsigned long lastMillis = 0;
AWSIoT awsIoT;
MYWIFI wifi;
void handleSerialDataAndPublish();
void handleButtonUpdate();
void setup()
{
  Serial.begin(4800);
  while(!wifi.connect(30)){}
  awsIoT.begin();

  pinMode(2,INPUT_PULLUP);



}

void loop()
{
  handleSerialDataAndPublish();
  handleButtonUpdate();
  awsIoT.loop();
  wifi.loop();
}

void handleSerialDataAndPublish(){
    // Check if data is available to read
  if (Serial.available() > 0)
  {
    String str = Serial.readStringUntil('\n');
    Serial.print("\n> Recieve: ");
    Serial.println(str);

    // Allocate buffer for JSON
    char *json = (char *)malloc(200);
    if (json == nullptr)
    {
      Serial.println("> Memory allocation failed");
    }
    else
    {
      // Convert to JSON
      if (convertDataToJson(str, json,200) == SUCCESS)
      {
        // Publish to AWS IoT
        awsIoT.publishMessage(json);
        Serial.print("> AWS Sent: ");
        Serial.println(json);
      }else{
        Serial.print("> ERROR: ");
        Serial.println("Cannot convert string to json");
      }

      free(json);
    }
  }



}
void handleButtonUpdate(){
  if(digitalRead(2) == 0){
    delay(1000);
    if(digitalRead(2) == 0){
      Serial.print("A");
    }
  }
}