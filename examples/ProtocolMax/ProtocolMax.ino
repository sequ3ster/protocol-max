/*
  ProtocolMax - Arduino sketch for communicating with EVBox Wallbox
  over RS232/485 (via Protocol Max).
  
  This file is part of ProtocolMax.
  https://github.com/sequ3ster/protocol-max
  
  ProtocolMax is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  ProtocolMax is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with ProtocolMax.  If not, see <http://www.gnu.org/licenses/>.
  
  Written by Benjamin Heimann
  Copyright © 2021 Benjamin Heimann
*/

#include "ProtocolMax.h" // Protocol Max pins   D7(13),D8(15)   RX,TX

// Instantiate Protocol Max object with Baudrate 38400
ProtocolMax node(38400);

void setup()
{
  delay(100);
  Serial.println("RS485 with Protocol Max Online");
}


void loop()
{
  int Mdelay = 15;   // microdelay
  node.SendCommand69(23, 14, 34, 60, 4, 8, 7);
}
