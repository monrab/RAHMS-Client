/**
 * @file APLib.cpp
 * @author Monika Rabka
 * @brief 
 * @version 0.1
 * @date 2021-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "APLib.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <sstream>
#include <iomanip>
#include <ArduinoJson.h>

const char *_ssid = "ESP32-AP";      // your server (access point) SSID
const char *_password = "123456789"; // your server (access point) password
unsigned long _lastTime = 0;
unsigned long _timerDelay = 5000;

//Constructor
APLib::APLib(bool displayMsg)
{

  Serial.println("Created the library object for APLib functions.");
}

// *************************** FUNCTIONS ***************************
// *****************************************************************
void APLib::wifiInit()
{
  //WiFi.begin(unconstchar(_ssid.c_str()), unconstchar(_password.c_str()));
  WiFi.begin("ESP32-AP", "123456789");
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.softAPIP());                                //get the AP IP address
  Serial.println("This station's IP address: " + WiFi.localIP()); //get the AP IP address
  _APAddress = WiFi.softAPIP().toString().c_str();                // stores the IP address of the access point where the Server will be hosting the website
}
// *****************************************************************
bool APLib::wifiCheck()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    return true;
  }
  else
  {
    return false;
    Serial.println("WiFi Disconnected");
  }
}
// *****************************************************************

// *****************************************************************
// GET request takes in the direct address of the webpage (http://xxx.xxx.xxx.xxx/directAddress)
String APLib::httpGETRequest(char *directAddress)
{

  HTTPClient http;
  String serverAddress = "http://" + _APAddress + directAddress;
  // Your IP address with path or Domain name with URL path
  http.begin(serverAddress);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "--";

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
// *****************************************************************
// POST request takes in the direct address of HTTP POST address of the webpage (http://xxx.xxx.xxx.xxx/directAddress)
// and the jsonString as the body of the HTTP POST request
void APLib::httpPOSTRequest(String directAddress, String jsonString)
{
  //Send an HTTP POST request every 5 seconds (_timerDelay)
  if ((millis() - _lastTime) > _timerDelay)
  {
    if (wifiCheck())
    {
      HTTPClient http;
      String serverAddress = "http://" + _APAddress + directAddress;
      Serial.println("Sending to: " + serverAddress);

      http.begin(serverAddress);
      http.addHeader("Content-Type", "application/json");

      // Send HTTP POST request
      int httpResponseCode = http.POST(jsonString);

      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      // Free resources
      http.end();
    }
    else
    {
      Serial.println("WiFi Disconnected. Trying to reconnect...");
      wifiInit();
    }
    _lastTime = millis();
  }
}
