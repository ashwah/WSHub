//
#include "ws_state.h"

WsState state;

void setup() {
  Serial.begin(115200);
  state.init();
}

void loop() {
  state.runState();
  delay(1);
}
