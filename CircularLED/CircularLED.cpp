/*
 2012 Copyright (c) Seeed Technology Inc.

 Author: LG
  
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc.,51 Franklin St,Fifth Floor, Boston, MA 02110-1301 USA

*/



#include "CircularLED.h"
#include <Arduino.h>

unsigned int CmdMode=0b0000000000000000;

void CircularLED::Sent16bit(unsigned int data)
{
   for(unsigned char i=0;i<16;i++)
  {
    if(data&0x8000)
      PORT_Data |= BIT_Data;
    else
      PORT_Data &=~ BIT_Data;

    PORT_Clk ^= BIT_Clk;
    data <<= 1;
  }
}
void CircularLED::CircularLEDWrite(unsigned int data[24])
{
  DDR_Data |= BIT_Data;
  DDR_Clk |= BIT_Clk;
  //PORT_Data &=~ BIT_Data;
  //PORT_Clk &=~ BIT_Clk;
  unsigned char count=0;
  Sent16bit(CmdMode);  
  for(count=0;count<12;count++)
  {
    Sent16bit(data[count]);
  }
  Sent16bit(CmdMode);  
  for(count=12;count<24;count++)
  {
    Sent16bit(data[count]);  
  }
  Latch();
}
void CircularLED::ClearDisplay()
{
  unsigned char i=0;
  unsigned char f=0;
  Sent16bit(CmdMode);  
  PORT_Data &=~ BIT_Data;
  for(i=0;i<192;i++)
  {
    PORT_Clk ^= BIT_Clk;
  }
  Sent16bit(CmdMode);  
  PORT_Data &=~ BIT_Data;
  for(i=0;i<192;i++)
  {
    PORT_Clk ^= BIT_Clk;
  }
  Latch();
  
}

void CircularLED::Latch(void)
{
  PORT_Data &=~ BIT_Data;

  delayMicroseconds(10);
  for(unsigned char i=0;i<8;i++)
  {
    PORT_Data ^= BIT_Data;
  } 
}
//CircularLED circularLED;
