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

#ifndef CircularLED_h
#define CircularLED_h

#define DDR_Data  DDRC
#define DDR_Clk   DDRC
#define PORT_Data  PORTC
#define PORT_Clk  PORTC
#define BIT_Data  0x10
#define BIT_Clk   0x20


class CircularLED
{
  public:
  void ClearDisplay(void);
  void CircularLEDWrite(unsigned int data[24]);
  private:
  void Sent16bit(unsigned int data);
  void Latch(void);
};
//extern CircularLED circularLED;

#endif;


