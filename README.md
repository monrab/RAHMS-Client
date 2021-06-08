# RAHMS-Client
This repo contains the .cpp files needed for the Real-time Autonomous Horticulture Monitoring System (RAHMS) Client.
![rahms client](https://github.com/monrab/assets/blob/main/RAHMS%20Client.jpeg?raw=true)

## Microcontroller and sensors used 
![client-circuit](https://github.com/monrab/assets/blob/main/Schematic_RAHMS_2021-06-08.png?raw=true)

* ESP32-WROOM32 - DevKit V1
* BME280 Air Humidity and Pressure sensor (breakout board)
* CCS811 eCO<sub>2</sub> and Total Volatile Compounds (tVOC) sensor (breakout board)
* DS18B20 waterproof temperature sensor
* FC28 Hygrometer
* GL55 Light Dependent Resistor

### Sensors.h and APLib.h
These two libraries have been written to keep the main sketch's code clean and readable.

In the APLib.h these two variables are for the AP (another ESP32 Server) connection.
```c++
const char *_ssid = "ESP32-AP";      // your server (access point) SSID
const char *_password = "123456789"; // your server (access point) password
```
To call a function from the Sensor.h these lines are used in the main sketch:

* to initialise the digital sensors:
```c++
 // Initialize all I2C sensors
  bool statusI2C = Wire.begin();
  bool statusBME280 = bme.begin(0x76);
  bool statusCCS811 = ccs811.begin();
  sensors.initializeSensor(statusI2C, "I2C");
  sensors.initializeSensor(statusBME280, "BME280"); // 
  sensors.initializeSensor(statusCCS811, "CCS811"); 

  // in DallasTemperature lib begin() function is a void so it can't be used as a boolean
  // so the Sensor lib funtion can't be used
  ds18b20.begin();
  Serial.println("DS18B20 started.");
```

* to read a sensor:
```c++
  sensors.readSensorDigital(ds18b20.getTempCByIndex(0), "DS18B20", "ºC"); // --> where first parameter is the digital read of the sensor, seconds is the sensor name String (for debugging), and third is the symbol (C, hPa etc. - also for debugging)
  sensors.readSensorAnalog(photoresistor, "LDR"); // --> where first parameter is the sensor pin that reads the ADC, and second parameter is the sensor name (for debugging)
```

## Parsing and sending JSON in HTTP POST request
sensorsJson is an Arduino_JSON.h object and the key-values can be assigned like so:
```c++
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
```
at the end a jsonString is created which is then used in the HTTP POST request.

In the loop() the following function from APLib.h deals with the HTTP POST request.
```c++
  // sensor library for HTTP POST request, with direct address to the server website  
  // receiving POST requests and JSON String as the body of the request
  aplib.httpPOSTRequest("/data", jsonString);
```
