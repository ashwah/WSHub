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

const String SERVER = "http://161.35.39.145:5000";
const String PATH_UUID = "/api/uuid";
const String PATH_WEIGHT_DATA = "/api/weight-data";

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

  http.end();
  return (httpResponseCode == 200);

}

void Api::postWeightData(String weight, String uuid)
{
  Serial.println("    Attempting post to API : ");
  http.begin(SERVER + PATH_WEIGHT_DATA);
  http.addHeader("Content-Type", "application/json");
  String json = "{\"pod_uuid\":\"" + uuid + "\",\"weight_value\":\"" + weight + "\"}";
  Serial.println(json);
  int httpResponseCode = http.POST(json);

  if (httpResponseCode == 200) {
    //String payload = http.getString();
    //payload.replace("\"", "");
    Serial.print("    Posted to API : ");//Serial.println(payload);
  }

  else {
    Serial.println("    Problem");
  }
  // Free resources
  http.end();

  return;

}

String Api::getUuid()
{

  http.begin(SERVER + PATH_UUID);
  int httpResponseCode = http.GET();

  if (httpResponseCode == 200) {
    String payload = http.getString();
    payload.replace("\"", "");
    Serial.print("    From API : ");Serial.println(payload);
    http.end();
    return payload;
  }

  else {
    Serial.println("    Problem");
  }
  // Free resources
  http.end();

  return "";

}
