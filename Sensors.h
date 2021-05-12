#ifndef sens
#define sens

#if (ARDUINO >=100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class SensorsLib {
  public:
    // constructor
    SensorsLib(bool displayMsg=false);

    void initializeSensor(bool sensorStatus, String sensorName);
    float readSensorAnalog(int sensor, String sensorName);
    float readSensorDigital(float sensor, String sensorname, String symbol);

  private:
  bool _debug;

};
#endif
