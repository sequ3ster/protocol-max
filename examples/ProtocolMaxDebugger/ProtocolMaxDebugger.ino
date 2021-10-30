/*
  Protocol Max Debugger

  This sketch sent and received data on the RS-485 interface with the Protocol Max

  Circuit:
   - MKR board
   - MKR 485 Shield
     - ISO GND connected to GND of the RS-485 device
     - Y connected to A of the RS-485 device
     - Z connected to B of the RS-485 device
     - A connected to Y of the RS-485 device
     - B connected to Z of the RS-485 device
     - Jumper positions
       - FULL set to ON
       - Z \/\/ Y set to ON, if the RS-485 device doesn't provide termination
       - B \/\/ A set to ON, if the RS-485 device doesn't provide termination

  created 30 Oktober 2021
  by Benjamin Heimann
*/

#include <ArduinoRS485.h>

#define PROTOCOLMAX_DEFAULT_BAUD 38400
#define PROTOCOLMAX_DEFAULT_CHARGEPOINT 0x80
#define PROTOCOLMAX_DEFAULT_SMARTGRID 0xA0
#define PROTOCOLMAX_DEFAULT_BROADCAST 0xBC
#define PROTOCOLMAX_DEFAULT_STX 0x02
#define PROTOCOLMAX_DEFAULT_ETX 0x03
#define PROTOCOLMAX_DEFAULT_ACK 0xAA00
#define PROTOCOLMAX_DEFAULT_NACK 0x0055

void setup() {
  Serial.begin(PROTOCOLMAX_DEFAULT_BAUD);
  RS485.begin(PROTOCOLMAX_DEFAULT_BAUD);

  // enable transmission, can be disabled with: RS485.endTransmission();
  RS485.beginTransmission();

  // enable reception, can be disabled with: RS485.noReceive();
  RS485.receive();
}

void loop() {
  if (Serial.available()) {
    RS485.write(Serial.read());
  }

  if (RS485.available()) {
    Serial.write(RS485.read());
  }
}

void SendCmd68() {

}

void SendCmd69() {

}
