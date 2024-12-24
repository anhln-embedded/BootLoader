#ifndef MY_WIFI_H_
#define MY_WIFI_H_
#include <Arduino.h>
#include <FS.h> 
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include "constant.h"

class MYWIFI{
public :
    MYWIFI();
    bool isConnected();
    bool connect(uint32_t timeLimit);
    void reset();
    void startAP();
    void stopAP();
    void loop();

private:
    ESP8266WebServer server;
    String readSSID();
    String readPassword();
    void saveWiFiCredentials(const String &ssid, const String &password);    

    // Helper function to initialize SPIFFS and start the web server
    void handleRoot();               // Handle requests to the root URL ("/")
    void handleSaveWiFi();           // Handle saving WiFi credentials from the form
};
#endif /*MY_WIFI_H_*/