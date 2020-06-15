#include "Arduino.h"
#include "ws_state.h"

#include "led.h"
#include "wifi_wrapper.h"
#include "api.h"

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
        _state = CHECK_API;
      }
      break;

    case WAIT_API:
      led.setColour(0, 0, 255);
      this->wait(5000, CHECK_API);
      break;

    case CHECK_API:
      led.setColour(0, 0, 255);  // blue
      Serial.println("WeightCore waiting for API");
      if (api.checkWifi()) {
        if (api.checkApi()) {
          _state = READY;
        }
        else {
          // API says no!
          _state = WAIT_API;
        }
      }
      else {
        // No wifi.
        _state = WAIT_WIFI;
      }
      break;

    case READY:
      led.setColour(0, 128, 128);  // blue+green
      // Serial.println("WeightCore ready");
      i2c.ready();
      if (i2c.hasNewWeight()) {
        _state = SENDING;
      }
      if (i2c.hasUuidRequest()) {
        _state = REQUESTING_UUID;
      }
      break;

    case SENDING:
      led.blinkColour(255, 0, 255); // Purple
      this->wait(4000, READY);
      break;

    case REQUESTING_UUID:
      led.setColour(255, 255, 255);
      Serial.println("    WeightCore requesting UUID");
      if (api.checkWifi()) {
        if (api.checkApi()) {
          String uuid = api.getUuid();
          delay(250);
          i2c.sendUuid(uuid);
          //this->wait(2000, READY);
          delay(1000);
          _state = READY;
        }
        else {
          // API says no!
          _state = WAIT_API;
        }
      }
      else {
        // No wifi.
        _state = WAIT_WIFI;
      }
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
