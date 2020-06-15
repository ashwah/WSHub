#include "Arduino.h"
#include "api.h"

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <HTTPClient.h>
#include <Arduino_JSON.h>

HTTPClient http;

const String SERVER = "https://weight-see.herokuapp.com";
const String PATH_UUID = "/api/uuid";

Api::Api()
{
}

bool Api::checkWifi()
{
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }
  else {
    return false;
  }
}

bool Api::checkApi()
{
  Serial.print("    Checking API, code : ");
  http.begin(SERVER + PATH_UUID);
  int httpResponseCode = http.GET();

  Serial.println(httpResponseCode);

  return (httpResponseCode == 200);

}

String Api::getUuid()
{

  http.begin(SERVER + PATH_UUID);
  int httpResponseCode = http.GET();

  if (httpResponseCode == 200) {
    String payload = http.getString();
    payload.replace("\"", "");
    Serial.print("    From API : ");Serial.println(payload);
    return payload;
  }

  else {
    Serial.println("    Problem");
  }
  // Free resources
  http.end();

  return "";

}
