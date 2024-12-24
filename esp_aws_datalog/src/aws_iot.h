#ifndef AWS_IOT_H
#define AWS_IOT_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "env.h"
#include "constant.h"

void messageReceived(char* topic, byte* payload, unsigned int length);

class AWSIoT {
public:
    AWSIoT(); 
    void begin();  
    void loop();  
    void publishMessage(char * msg); 
    void subscribeToTopic(const char* topic);  
    void setCallback();  

private:

    void NTPConnect(); 
    status_t checkWiFi(); 
    void connectAWS(); 

    WiFiClientSecure net;
    BearSSL::X509List cert;
    BearSSL::X509List client_crt;
    BearSSL::PrivateKey key;
    PubSubClient client;

};

#endif
