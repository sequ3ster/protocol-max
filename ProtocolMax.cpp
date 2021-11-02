/**
@file
Arduino library for communicating with EVBox Wallbox over RS232/485 (via Protocol Max).
*/
/*
  ProtocolMax.cpp - Arduino library for communicating with EVBox Wallbox
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
  Copyright © 2009-2013 Benjamin Heimann
*/

#include "ProtocolMax.h"


unsigned long LastReceive1;  

/* _____PUBLIC FUNCTIONS_____________________________________________________ */

/**
Constructor.
Creates class object using default serial port 0.
@ingroup setup
*/
ProtocolMax::ProtocolMax()
{
  begin(38400);
}

/**
Constructor.
Creates class object using default serial port 0.
@overload ProtocolMax::ProtocolMax(unsigned long BaudRate)
@param BaudRate baud rate, in standard increments (300..115200)
@ingroup setup
*/
ProtocolMax::ProtocolMax(unsigned long BaudRate)
{
  begin(BaudRate);
}

/**
Initialize class object.

Sets up the serial port using default 38400 baud rate.
Call once class has been instantiated, typically within setup().

@ingroup setup
*/
void ProtocolMax::begin(void)
{  
  begin(38400);
}


/**
Initialize class object.

Sets up the Serial1 port using specified baud rate.
Call once class has been instantiated, typically within setup().

@overload ProtocolMax::begin(unsigned long BaudRate)
@param BaudRate baud rate, in standard increments (300..115200)
@ingroup setup
*/
void ProtocolMax::begin(unsigned long BaudRate )
{
	delay(100);  
	Serial1.begin(BaudRate, SERIAL_8N1);

  // Debug
  Serial.begin(BaudRate, SERIAL_8N1);
}


void ProtocolMax::SendCommand68()
{
  //<STX>80A0680192305400112233445566778899AABBCCDDEEFF00E6008C014003C002800500046F703<ETX>
  // SendMessage(_PROTOCOLMAX_CHARGEPOINT, _PROTOCOLMAX_SMARTGRID, _PROTOCOLMAX_COMMAND68, 'MESSAGE');
}

void ProtocolMax::SendCommand69(float SetMaxL1, float SetMaxL2, float SetMaxL3, uint16_t Timeout, float MaxL1, float MaxL2, float MaxL3)
{
  // Example: SendCommand69(23,14,34,60,4,8,7);
  // "<STX>80A06900E6008C0154003C002800500046F703<ETX>"
  //    |  | | |   |   |   |   |   |   |   | | |   |__ <ETX>  End byte
  //    |  | | |   |   |   |   |   |   |   | | |______ 03     CheckSum8 Xor
  //    |  | | |   |   |   |   |   |   |   | |________ F7     CheckSum8 Modulo 256
  //    |  | | |   |   |   |   |   |   |   |__________ 0046   7.0A L3 Max Ampere after Timeout
  //    |  | | |   |   |   |   |   |   |______________ 0050   8.0A L2 Max Ampere after Timeout
  //    |  | | |   |   |   |   |   |__________________ 0028   4.0A L1 Max Ampere after Timeout
  //    |  | | |   |   |   |   |______________________ 003C   60 Seconds Timeout
  //    |  | | |   |   |   |__________________________ 0154   34.0A L3 Max Ampere
  //    |  | | |   |   |______________________________ 008C   14.0A L2 Max Ampere
  //    |  | | |   |__________________________________ 00E6   23.0A L1 Max Ampere
  //    |  | | |______________________________________ 69     Command 69
  //    |  | |________________________________________ A0     source address "SmartGrid-module"
  //    |  |__________________________________________ 80     dest address "ChargePoint-module"
  //    |_____________________________________________ <STX>  Start byte
  
  char DestAddress[] = _PROTOCOLMAX_CHARGEPOINT;
  char SourceAddress[] = _PROTOCOLMAX_SMARTGRID;
  char Command[] = _PROTOCOLMAX_COMMAND69;    
  char Data[120];   
  char CSetMaxL1[5];
  char CSetMaxL2[5];
  char CSetMaxL3[5];
  char CTimeout[5];
  char CMaxL1[5];
  char CMaxL2[5];
  char CMaxL3[5];
  
  FloatToHex(SetMaxL1, CSetMaxL1);  
  FloatToHex(SetMaxL2, CSetMaxL2);
  FloatToHex(SetMaxL3, CSetMaxL3);
  IntToHex(Timeout, CTimeout);
  FloatToHex(MaxL1, CMaxL1);
  FloatToHex(MaxL2, CMaxL2);
  FloatToHex(MaxL3, CMaxL3);

  strcpy(Data, DestAddress);
  strcat(Data, SourceAddress);
  strcat(Data, Command);  
  strcat(Data, CSetMaxL1);
  strcat(Data, CSetMaxL2);
  strcat(Data, CSetMaxL3);
  strcat(Data, CTimeout);
  strcat(Data, CMaxL1);
  strcat(Data, CMaxL2);
  strcat(Data, CMaxL3);
  strcat(Data, "\0");
  
  SendMessage(DestAddress, SourceAddress, Command, Data);
}

void ProtocolMax::SendMessage(char DestAddress[3], char SourceAddress[3], char Command[3], char* Data)
{
  char crc[5]; 
  Checksum(Data,crc);    

  // while(((_u8LastReceive1 + _PROTOCOLMAX_TX_TIMEOUT) > millis()) or (Serial1.available()))
  while(((LastReceive1 + _PROTOCOLMAX_TX_TIMEOUT) > millis()) or (Serial1.available()))
  {  
    delay(10);
  }

  Serial1.write(_PROTOCOLMAX_STX) ;
  Serial1.print(DestAddress);
  Serial1.print(SourceAddress);
  Serial1.print(Command);
  Serial1.print(Data);
  Serial1.print(crc);
  Serial1.write(_PROTOCOLMAX_ETX) ;

  LastReceive1 = millis();

  //Debug
  Serial.write(_PROTOCOLMAX_STX) ;
  Serial.print(DestAddress);
  Serial.print(SourceAddress);
  Serial.print(Command);
  Serial.print(Data);
  Serial.print(crc);
  Serial.write(_PROTOCOLMAX_ETX) ;
}

void ProtocolMax::Checksum(char *s, char crc[5])
{
  uint8_t ModCrcByte = ModChecksum(s);
  uint8_t XorCrcByte = XorChecksum(s);
  
  char ModCrc[2];  
  char XorCrc[2];

  ChecksumString(ModCrcByte,ModCrc);
  ChecksumString(XorCrcByte,XorCrc);
  
  crc[0] = ModCrc[0];
  crc[1] = ModCrc[1];
  crc[2] = XorCrc[0];
  crc[3] = XorCrc[1];
  crc[4] = '\0';
}


uint8_t ProtocolMax::available(void)
{
  return _u8ResponseBufferLength - _u8ResponseBufferIndex;
}


/* _____PRIVATE FUNCTIONS____________________________________________________ */

void ProtocolMax::FloatToHex(float Value, char hex[5])
{     
    uint16_t IntValue = Value * 10;
    IntToHex(IntValue, hex);
}

void ProtocolMax::IntToHex(uint16_t Value, char hex[5])
{     
    uint16_t buffer = Value;

    int n = buffer / 4096;
    buffer -= n * 4096;
    int i = buffer / 256;
    buffer -= i * 256;
    int j = buffer / 16;
    buffer -= j * 16;
    int k = buffer;

    char cn = n;
    char ci = i;
    char cj = j;
    char ck = k;

    if (cn < 10) cn+='0';
    else cn+='A'-10;
    if (ci < 10) ci+='0';
    else i+='A'-10;
    if (cj < 10) cj+='0';
    else cj+='A'-10;
    if (ck < 10) ck+='0';
    else ck+='A'-10;
  
    hex[0] = cn;
    hex[1] = ci;
    hex[2] = cj;
    hex[3] = ck;  
    hex[4] = '\0';
}


void serialEvent1()
{
  int incomingByte = Serial.read();

  // prints the received data
  Serial.print("I received: ");
  Serial.println((char)incomingByte);
  LastReceive1 = millis();
}

void serialEvent()
{
  int incomingByte = Serial.read();

  // prints the received data
  Serial.print("I received: ");
  Serial.println((char)incomingByte);
  LastReceive1 = millis();
}

uint8_t ProtocolMax::XorChecksum(char *s)
{
    uint8_t crc = 0;
    while(*s != '\0')
        crc ^= *s++; 
    return crc;
}

uint8_t ProtocolMax::ModChecksum(char *s)
{
  int i;
  uint8_t summe = 0x00;
  
  for (i = 0; i < strlen(s); i++) {
    summe += s[i];
  }
    
  return summe % 256;
}

void ProtocolMax::ChecksumString(uint8_t CrcByte, char crc[3])
{   
    char j = CrcByte / 16;
    char k = CrcByte - 16 * j;
    if (j < 10) j+='0';
    else j+='A'-10;
    if (k < 10) k+='0';
    else k+='A'-10;
  
    crc[0] = j;
    crc[1] = k;
    crc[2] = '\0';
}