/**
@file
Arduino library for communicating with EVBox Wallbox over RS232/485 (via Protocol Max).
*/
/*

  ProtocolMax.h - Arduino library for communicating with EVBox Wallbox
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
  
#ifndef	_PROTOCOLMAX_LIBRARY_H
#define	_PROTOCOLMAX_LIBRARY_H
#define _PROTOCOLMAX_STX 0x02
#define _PROTOCOLMAX_ETX 0x03
#define _PROTOCOLMAX_ACK 0xAA00
#define _PROTOCOLMAX_NACK 0x0055
#define _PROTOCOLMAX_CHARGEPOINT "80" //0x80
#define _PROTOCOLMAX_SMARTGRID "A0" //0xA0
#define _PROTOCOLMAX_BROADCAST "BC" //0xBC
#define _PROTOCOLMAX_COMMAND68 "68" //0x36 + 0x38
#define _PROTOCOLMAX_COMMAND69 "69" //0x36 + 0x39
#define _PROTOCOLMAX_TX_TIMEOUT 100 


/* _____STANDARD INCLUDES____________________________________________________ */

#include <String.h>  
#include <Arduino.h>


/* _____CLASS DEFINITIONS____________________________________________________ */
/**
Arduino class library for communicating with Protocol Max over RS232/485.
*/
class ProtocolMax
{
  public:
  
    ProtocolMax();        
    ProtocolMax(unsigned long);   
    void begin();
    void begin(unsigned long);
    
    void SendCommand68();
    void SendCommand69(float SetMaxL1, float SetMaxL2, float SetMaxL3, uint16_t Timeout, float MaxL1, float MaxL2, float MaxL3);
    void SendMessage(char[], char[], char[], char* data);
    
    void Checksum(char *s, char crc[5]);
    uint8_t available(void);   
    
  private:

    unsigned long _u8LastReceive1;                                    ///< Milliseconds of the last received Message on Serial    
    uint16_t _u16BaudRate;                                       ///< baud rate (300..115200) initialized in begin()   
    uint8_t _u8ResponseBufferIndex;
    uint8_t _u8ResponseBufferLength;                

    void FloatToHex(float Value, char hex[5]);
    void IntToHex(uint16_t Value, char hex[5]);      
    uint8_t XorChecksum(char *s);
    uint8_t ModChecksum(char *s);
    void ChecksumString(uint8_t CrcByte, char crc[3]);

};
#endif

/**
@example examples/ProtocolMax/ProtocolMax.ino
*/
