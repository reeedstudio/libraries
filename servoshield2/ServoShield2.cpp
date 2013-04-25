/*
  ServoShield2.cpp - PCA9685 based servo shield for Arduino's to control up to 16 servos
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
#include <ServoShield2.h>
#include <Wire.h>
#include <inttypes.h>

ServoShield2::ServoShield2(uint8_t addr, uint8_t mode) {
  address = addr;
  
  if (mode == 60)
  {
    prescale = 106;
  }
  else prescale = 127;
  
  for (int i = 0; i < 16; i++)
    invertedservos[i] = false;
}

uint8_t ServoShield2::readbyte(uint8_t addr) {
  Wire.beginTransmission(address);
  Wire.write(addr);
  Wire.endTransmission();
  Wire.requestFrom(address, (uint8_t)1);
  return Wire.read();
}

void ServoShield2::writebyte(uint8_t addr, uint8_t value) {
  Wire.beginTransmission(address);
  Wire.write(addr);
  Wire.write(value);
  Wire.endTransmission();
}

void ServoShield2::setposition(int servo, int position) {
  
  if (position > servosmax[servo])
    position = servosmax[servo];
    
  if (position < servosmin[servo])
    position = servosmin[servo];
    
  if (invertedservos[servo]) {
      
  }
  
  position = floor(position / 4.88);
  writebyte(LED_ON_L + 4 * servo, 0);
  writebyte(LED_ON_H + 4 * servo, 0 >> 8);
  writebyte(LED_OFF_L + 4 * servo, position);
  writebyte(LED_OFF_H + 4 * servo, position >> 8);  
}

boolean ServoShield2::start(void) {
  Wire.begin();
  
  writebyte(MODE1REG, 0x0);
  uint8_t mode1regvalue = readbyte(MODE1REG);
  uint8_t newregvalue = (mode1regvalue & 0x7F) | 0x10;    
  writebyte(MODE1REG, newregvalue);            
  writebyte(PRE_SCALEREG, prescale);   
  writebyte(MODE1REG, mode1regvalue);
  delay(5);
  writebyte(MODE1REG, mode1regvalue | 0x80);
  return true;
}

int ServoShield2::setbounds(int servo, int minposition, int maxposition)
{
  if (servo < 16)
  {
    servosmax[servo] = maxposition;
    servosmin[servo] = minposition;
    return 0;
  }
	
  return 1;
}

int ServoShield2::getposition(int servo)
{
  return 0;
}

int ServoShield2::invertservo(int servo)
{
  invertedservos[servo] = true;
  return 1;
}

