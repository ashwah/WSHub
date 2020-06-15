#include "Arduino.h"
#include "i2c.h"

#include <Wire.h>

const int I2C_SDA = 21;
const int I2C_SCL = 22;

const int MIN_ADDRESS = 1;
const int MAX_ADDRESS = 99;

const int FULL_SCAN_INTERVAL = 10000;
const int ADDRESS_SCAN_INTERVAL = 500;

const int WEIGHT_LENGTH = 5;
const int UUID_LENGTH = 36;

I2c::I2c()
{
  Wire.begin(I2C_SDA, I2C_SCL);
  _timer_a = millis();
  _timer_f = millis();
}

void I2c::ready()
{
  if (((int) millis() - _timer_f) > FULL_SCAN_INTERVAL) {
    this->scanFull();
    _timer_f = millis();
  }
  if (((int) millis() - _timer_a) > ADDRESS_SCAN_INTERVAL) {
    this->scanAddresses();
    _timer_a = millis();
  }
}

void I2c::scanAddresses()
{
  Serial.println("Scanning...");
  for (byte address = MIN_ADDRESS; address < MAX_ADDRESS; ++address) {
    if (_addresses[address] && !_has_new_weight && !_has_uuid_request) {
      this->processMessage(address);
    }
  }
}

void I2c::scanFull()
{
  Serial.println("Scanning all addresses...");

  for (byte address = MIN_ADDRESS; address < MAX_ADDRESS; ++address) {
    // Use the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      _addresses[address] = true;
      Serial.print("  I2C device found at address ");
      Serial.println(address);
    }
    else {
      _addresses[address] = false;
    }
  }
}

void I2c::processMessage(int address) {
  Wire.requestFrom(address, 1 + WEIGHT_LENGTH + UUID_LENGTH);
  Serial.print("  Message from ");
  Serial.print(address);
  Serial.print(" : ");

  //char message[1];//MESSAGE_LENGTH];
  char code;
  int i = 0;
  while(Wire.available() && i < 1) //MESSAGE_LENGTH)    // slave may send less than requested
  {
    code = Wire.read(); // receive a byte as
    Serial.print(code);

    //New weight data.
    if (code == '1') {
      Serial.println("  *********************weight***********");
      _has_new_weight = true;

      char weight[WEIGHT_LENGTH];
      i = 0;
      Serial.print("    Weight : ");
      while(Wire.available() && i < WEIGHT_LENGTH) {
        weight[i] = Wire.read();
        Serial.print(weight[i]);
        i++;
      }
      Serial.println();

      //char uuid[UUID_LENGTH];
      String uuid;
      char c;
      i = 0;
      Serial.print("    UUID : ");
      while(Wire.available() && i < UUID_LENGTH) {
        c = Wire.read();
        uuid += c;
        i++;
      }

      Serial.println(uuid);

      while(Wire.available()) {
        Serial.println(Wire.read());
      }

      this->acknowldge(address, '1');
      return;
    }

    // New device on i2c.
    if (code == '2') {
      Serial.println("  *********************i2c handshake***********");
      this->acknowldge(address, '2');
      return;
    }


    // UUID requested.
    if (code == '3') {
      _has_uuid_request = true;
      _uuid_request_address = address;
      Serial.println("  *********************uuid request***********");
      /////
      return;
    }

    i++;
  }

  Serial.println();
}

bool I2c::hasNewWeight() {
  if (_has_new_weight) {
    _has_new_weight = false;
    return true;
  }
  else {
    return false;
  }
}

bool I2c::hasUuidRequest() {
  if (_has_uuid_request) {
    _has_uuid_request = false;
    return true;
  }
  else {
    return false;
  }
}

void I2c::sendUuid(String uuid) {
  Serial.print("  Sending UUID to ");
  Serial.println(_uuid_request_address);
  char code = 3;
  Wire.beginTransmission(_uuid_request_address);
  char buf[37];
  uuid.toCharArray(buf, 37);
  Wire.write("3");
  Wire.write(buf);
  Wire.endTransmission();
}

void I2c::acknowldge(int address, char code) {
  Serial.println("  Acknowledging.");
  Wire.beginTransmission(address);
  Wire.write(code);
  Wire.endTransmission();
}
