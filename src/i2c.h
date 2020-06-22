#ifndef I2c_h
#define I2c_h

#include "Arduino.h"
#include <Wire.h>

class I2c
{
  public:
    I2c();
    void ready();
    void scanAddresses();
    void scanFull();
    void processMessage(int address);
    bool hasNewWeight();
    String getNewWeight();
    String getNewWeightUuid();
    bool hasUuidRequest();
    void acknowldge(int address, char code);
    void sendUuid(String uuid);

  private:
    unsigned int _timer_a;
    unsigned int _timer_f = -5000;
    bool _addresses[127] = {};
    bool _has_new_weight;
    String _new_weight;
    String _new_weight_uuid;
    bool _has_uuid_request;
    int _uuid_request_address;
};

#endif
