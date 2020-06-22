#ifndef Api_h
#define Api_h

#include "Arduino.h"
#include <Wire.h>

class Api
{
  public:
    Api();
    bool checkWifi();
    bool checkApi();
    String getUuid();
    void postWeightData(String weight, String uuid);

  private:
    String _server;
};

#endif
