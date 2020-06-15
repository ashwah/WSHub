#ifndef WsState_h
#define WsState_h

#include "Arduino.h"
#include "led.h"
#include "wifi_wrapper.h"
#include "i2c.h"
#include "api.h"

class WsState
{
  public:
    WsState();
    void init();
    void runState();
    void wait(int wait_time, uint8_t transition_state);
    Led led;
    WifiWrapper wifiWrapper;
    I2c i2c;
    Api api;

  private:
    enum State_enum {INIT, WAIT_WIFI, CHECK_API, WAIT_API, READY, SENDING, REQUESTING_UUID, ERROR_STATE};
    uint8_t _state;
    unsigned long _timer;
    bool _waiting = false;
};

#endif
