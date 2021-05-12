/**
 * @file Sensors.cpp
 * @author Monika Rabka
 * @brief 
 * @version 0.1
 * @date 2021-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "Sensors.h"

//Constructor
SensorsLib::SensorsLib(bool displayMsg)
{
  _debug = displayMsg;
  Serial.println("Created the library object for sensors functions.");
}

// *************************** FUNCTIONS ***************************
// *****************************************************************
void SensorsLib::initializeSensor(bool sensorStatus, String sensorName)
{
  if (_debug)
  {
    if (!sensorStatus)
    {
      Serial.println("Could not find " + sensorName + ", check wiring!");
    }
    else
    {
      Serial.println(sensorName + " started.");
    }
  }
}
// *****************************************************************
// Reads the analog pin of the sensor and maps the value of the
// the ADC to 0-100%
float SensorsLib::readSensorAnalog(int sensor, String sensorName)
{
  String sensorNme = sensorName;
  int sensorValue = analogRead(sensor);
  int mappedValue = map(sensorValue, 0, 4095, 0, 100);
  if (sensorNme == "FC28")
  {
    mappedValue = (100 - ((sensorValue / 4095.00) * 100));
  }
  if (isnan(sensor))
  {
    if (_debug)
    {
      Serial.println("Failed to read " + sensorNme);
    }
    return 0;
  }
  else
  {
    if (_debug)
    {
      Serial.println(sensorNme + ": " + mappedValue + "%.");
    }
    return mappedValue;
  }
  delay(10);
}
// *****************************************************************
// Reads a digital sensor and returns the read value
// takes in the sensor name, symbol (*C, F, hPa etc.), and the reading of the sensor
float SensorsLib::readSensorDigital(float sensor, String sensorName, String symbol)
{
  if (isnan(sensor))
  {
    if (_debug)
    {
      Serial.println("Failed to read " + sensorName);
    }
    return 0;
  }
  else
  {
    if (_debug)
    {
      Serial.println(sensorName + ": " + sensor + symbol + ".");
    }
    return sensor;
  }
  delay(10);
}
// ******************************************************************
