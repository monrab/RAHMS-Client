/**
 * @file SENSORS_CLIENT_v1.ino
 * @author Monika Rabka
 * @brief 
 * @version 0.1
 * @date 2021-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "Sensors.h"
#include "APLib.h"

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "SparkFunCCS811.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Arduino_JSON.h>

//declaring sensor pins
const int ds18b20pin = 4;
const int hygrometer = 34;
const int photoresistor = 35;
Adafruit_BME280 bme;
CCS811 ccs811(0x5A);
OneWire oneWire(ds18b20pin); //if other temp sensors are gonna be used
DallasTemperature ds18b20(&oneWire);

//constructor for the lib classes
SensorsLib sensors(true); //true displays Serial, false no messages
APLib aplib;

// **************************************
JSONVar sensorsJson;
String jsonString;
// ***************************************

void readSensors()
{
  ds18b20.requestTemperatures();
  ccs811.readAlgorithmResults();

  // library functions for digital and analog sensors,
  // take in digital values read from digital sensors or analog pins of analog sensors
  sensors.readSensorDigital(bme.readTemperature(), "BME280 Temperature", "ºC");
  sensors.readSensorDigital(bme.readHumidity(), "BME280 Humidity", "%");
  sensors.readSensorDigital((bme.readPressure() / 100.0F), "BME280 Pressure", "hPa");
  sensors.readSensorDigital(ccs811.getCO2(), "CCS811 CO2", "ppm");
  sensors.readSensorDigital(ccs811.getTVOC(), "CCS811 TVOC", "ppb");
  sensors.readSensorDigital(ds18b20.getTempCByIndex(0), "DS18B20", "ºC");
  sensors.readSensorAnalog(photoresistor, "LDR");
  sensors.readSensorAnalog(hygrometer, "FC28");
}

void parseValues()
{

  //Create a JSON formatted object
  sensorsJson["id"] = "1";
  sensorsJson["airtemperature"] = sensors.readSensorDigital(bme.readTemperature(), "BME280 Temperature", "ºC");
  sensorsJson["airhumidity"] = sensors.readSensorDigital(bme.readHumidity(), "BME280 Humidity", "%");
  sensorsJson["airpressure"] = sensors.readSensorDigital((bme.readPressure() / 100.0F), "BME280 Pressure", "hPa");
  sensorsJson["eCO2"] = sensors.readSensorDigital(ccs811.getCO2(), "CCS811 CO2", "ppm");
  sensorsJson["tVOC"] = sensors.readSensorDigital(ccs811.getTVOC(), "CCS811 TVOC", "ppb");
  sensorsJson["soiltemperature"] = sensors.readSensorDigital(ds18b20.getTempCByIndex(0), "DS18B20", "ºC");
  sensorsJson["soilmoisture"] = sensors.readSensorAnalog(hygrometer, "FC28");
  sensorsJson["LDR"] = sensors.readSensorAnalog(photoresistor, "LDR");

  //change the JSON object into a String
  jsonString = JSON.stringify(sensorsJson);
  Serial.println(jsonString);

  /** Generate the minified JSON and print it out Serial port.
    Serial.println("JSON is created:");
    serializeJson(sensorsJson, Serial);
  */
}

void setup()
{
  Serial.begin(115200);

  // Initialize all sensors
  bool statusI2C = Wire.begin();
  bool statusBME280 = bme.begin(0x76);
  bool statusCCS811 = ccs811.begin();
  sensors.initializeSensor(statusI2C, "I2C");
  sensors.initializeSensor(statusBME280, "BME280");
  sensors.initializeSensor(statusCCS811, "CCS811");

  // in DallasTemperature lib begin() function is a void so it can't be used as a boolean
  // so the Sensor lib funtion can't be used
  ds18b20.begin();
  Serial.println("DS18B20 started.");

  aplib.wifiInit();
}

void loop()
{
  if (!aplib.wifiCheck())
  {
    aplib.wifiInit();
  }
  ds18b20.requestTemperatures();
  ccs811.readAlgorithmResults();
  //readSensors();
  parseValues();
  // sensor library for HTTP POST request, with direct address and JSON String as the body of the request
  aplib.httpPOSTRequest("/data", jsonString);
  delay(5000);
}
