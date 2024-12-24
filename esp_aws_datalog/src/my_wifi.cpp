#include "my_wifi.h"

#define EEPROM_SIZE 64
#define SSID_ADDRESS 0
#define PASSWORD_ADDRESS 32

MYWIFI::MYWIFI() : server(80)
{
    EEPROM.begin(EEPROM_SIZE);                                               // Initialize EEPROM
    server.on("/", HTTP_GET, std::bind(&MYWIFI::handleRoot, this));          // Serve the HTML page
    server.on("/save", HTTP_POST, std::bind(&MYWIFI::handleSaveWiFi, this)); // Save WiFi credentials
}
bool MYWIFI ::isConnected()
{
    return WiFi.isConnected();
}
String MYWIFI::readSSID()
{
    char ssid[32];
    for (int i = 0; i < 32; i++)
    {
        ssid[i] = EEPROM.read(SSID_ADDRESS + i);
    }
    return String(ssid);
}

String MYWIFI::readPassword()
{
    char password[32];
    for (int i = 0; i < 32; i++)
    {
        password[i] = EEPROM.read(PASSWORD_ADDRESS + i);
    }
    return String(password);
}

void MYWIFI::saveWiFiCredentials(const String &ssid, const String &password)
{
    for (int i = 0; i < 32; i++)
    {
        EEPROM.write(SSID_ADDRESS + i, i < ssid.length() ? ssid[i] : 0);
        EEPROM.write(PASSWORD_ADDRESS + i, i < password.length() ? password[i] : 0);
    }
    EEPROM.commit();
}

bool MYWIFI::connect(uint32_t timeLimit)
{
    // String ssid = readSSID();
    // String password = readPassword();

    // if (ssid.length() > 0 && password.length() > 0)
    // {
    //     WiFi.begin(ssid.c_str(), password.c_str());
    // }else{
    //     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);        
    // }
    Serial.println("Connecting to WiFi");
    
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print('.');
    }
    return WiFi.isConnected();
}

void MYWIFI::reset()
{
    for (int i = 0; i < EEPROM_SIZE; i++)
    {
        EEPROM.write(i, 0);
    }
    EEPROM.commit();
    WiFi.disconnect();
    Serial.println("WiFi credentials reset!");
}

void MYWIFI::startAP()
{
    WiFi.softAP("ESP_Config_AP", "12345678");
    Serial.println("Access Point started.");
    Serial.print("AP IP Address: ");
    Serial.println(WiFi.softAPIP());

    // Initialize SPIFFS
    if (!SPIFFS.begin())
    {
        Serial.println("SPIFFS mount failed!");
        return;
    }
}

void MYWIFI::stopAP()
{
    WiFi.softAPdisconnect(true);
    Serial.println("Access Point stopped.");
}
void MYWIFI::handleRoot()
{
    File file = SPIFFS.open("/index.html", "r");
    if (file)
    {
        server.streamFile(file, "text/html");
        file.close();
    }
    else
    {
        server.send(404, "text/plain", "File not found");
    }
}

void MYWIFI::handleSaveWiFi()
{
    String ssid = server.arg("ssid");
    String password = server.arg("password");

    if (!ssid.isEmpty() && !password.isEmpty())
    {
        saveWiFiCredentials(ssid, password);
        server.send(200, "text/html", "<h1>WiFi Saved! Restarting...</h1>");
        delay(2000);
        ESP.restart(); // Restart ESP after saving credentials
    }
    else
    {
        server.send(400, "text/html", "<h1>Error: SSID and Password cannot be empty!</h1>");
    }
}

void MYWIFI ::loop()
{
    if (!isConnected()) // wifi not available
    {
        if (WiFi.softAPgetStationNum() == 0)
        {
            startAP();
            server.begin(); // Start the web server
            Serial.println("Web server started.");
        }
        server.handleClient();
    }
    else
    {
        if (WiFi.softAPgetStationNum() > 0)
        {
            stopAP();
            server.stop();
        }
    }
}