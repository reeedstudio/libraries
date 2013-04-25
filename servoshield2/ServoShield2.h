/*
  ServoShield2.h - PCA9685 based servo shield for Arduino's to control up to 16 servos
  Revision 1
  Copyright (c) 2012 Adriaan Swanepoel.  All right reserved.

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
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef ServoShield2_h
#define ServoShield2_h

#define ServoShieldVersion 2.0

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <inttypes.h>

#define MODE1REG 0x0
#define PRE_SCALEREG 0xFE
#define LED_ON_L 0x6
#define LED_ON_H 0x7
#define LED_OFF_L 0x8
#define LED_OFF_H 0x9

class ServoShield2 {
 public:
  ServoShield2(uint8_t addr = 0x40, uint8_t mode = 50);
  void setposition(int servo, int position);
  int setbounds(int servo, int minposition, int maxposition);
  int getposition(int servo);	
  int invertservo(int servo);
  boolean start(void);

 private:
  uint16_t servosmax[16];
  uint16_t servosmin[16];
  boolean invertedservos[16];
  uint8_t address;
  uint8_t prescale;
  uint8_t readbyte(uint8_t addr);
  void writebyte(uint8_t addr, uint8_t value);
};

#endif
