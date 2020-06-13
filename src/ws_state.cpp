#include "Arduino.h"
#include "ws_state.h"

#include "led.h"
#include "wifi_wrapper.h"

// RGB must be pins that function as analog.
const byte rrr = 25;
const byte ggg = 33;
const byte bbb = 32;

// Define pins for scales.
const int CLK = 8;
const int DOUT = 7;

int currentMillis;

WsState::WsState()
{
}

void WsState::init() {
  led.init(rrr, ggg, bbb);
}

void WsState::runState()
{
  switch(_state)
  {
    case INIT:
      led.setColour(255, 64, 0);  // ?
      this->wait(2000, WAIT_WIFI);
      Serial.println("WeightCore initalizing");
      break;

    case WAIT_WIFI:
      led.setColour(194, 32, 0);  // blue+green
      Serial.println("WeightCore waiting for WiFi");
      if (wifiWrapper.autoConnect()) {
        _state = READY;
      }
      break;

    case READY:
      led.setColour(0, 128, 128);  // blue+green
      // Serial.println("WeightCore ready");
      i2c.ready();
      if (i2c.hasNewWeight()) {
        _state = SENDING;
      }
      break;

    case SENDING:
      led.blinkColour(255, 0, 255);
      this->wait(4000, READY);
      break;
  }
}

/*
 * Provides a non-blocking wait method.
 */
void WsState::wait(int wait_time, uint8_t transition_state) {
  if (!_waiting) {
    _waiting = true;
    _timer = millis();
  }
  else {
    if (((int) (millis() - _timer)) > wait_time) {
      _state = transition_state;
      _waiting = false;
      _timer = millis();
    }
  }
}
