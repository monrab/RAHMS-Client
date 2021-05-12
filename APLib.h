#ifndef wi
#define wi

#if (ARDUINO >=100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class APLib {
  public:
    // constructor
    APLib(bool displayMsg=false);
    // functions
    void wifiInit();
    bool wifiCheck();
    String convertToString(float mfloat);
    String httpGETRequest(char* directAddress);
    void httpPOSTRequest(String directAddress, String jsonString);
    char *unconstchar(const char *s);
    
  private:

    String _ssid;
    String _password;
    String _APAddress;
    unsigned long _lastTime = 0;
    unsigned long _timerDelay = 5000;
  
};
#endif
