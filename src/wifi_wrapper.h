#ifndef WifiWrapper_h
#define WifiWrapper_h

#include "Arduino.h"

class WifiWrapper
{
  public:
    WifiWrapper();
    bool autoConnect();

  private:
    unsigned int _timer = -10000;
};

#endif
