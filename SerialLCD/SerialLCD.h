/*
  SerialLCD.h - Serial LCD driver Library
  2010 Copyright (c) Seeed Technology Inc.  All right reserved.
 
  Original Author: Jimbo.We
  Contribution: Visweswara R 
  
  Modified 15 March,2012 for Arduino 1.0 IDE
  by Frankie.Chu
  
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

#ifndef SERIAL_LCD_H
#define SERIAL_LCD_H

#include <inttypes.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

//Initialization Commands or Responses

#define SLCD_INIT	0xA3
#define SLCD_INIT_ACK	0xA5
#define SLCD_INIT_DONE	0xAA

//WorkingMode Commands or Responses
#define SLCD_CONTROL_HEADER	0x9F
#define SLCD_CHAR_HEADER	0xFE
#define SLCD_CURSOR_HEADER	0xFF
#define SLCD_CURSOR_ACK		0x5A

#define SLCD_RETURN_HOME	0x61
#define SLCD_DISPLAY_OFF	0x63
#define SLCD_DISPLAY_ON		0x64
#define SLCD_CLEAR_DISPLAY	0x65
#define SLCD_CURSOR_OFF		0x66
#define SLCD_CURSOR_ON		0x67
#define SLCD_BLINK_OFF		0x68
#define SLCD_BLINK_ON		0x69
#define SLCD_SCROLL_LEFT	0x6C
#define SLCD_SCROLL_RIGHT	0x72
#define SLCD_NO_AUTO_SCROLL	0x6A
#define SLCD_AUTO_SCROLL	0x6D
#define SLCD_LEFT_TO_RIGHT	0x70
#define SLCD_RIGHT_TO_LEFT	0x71
#define SLCD_POWER_ON    	0x83
#define SLCD_POWER_OFF  	0x82
#define SLCD_INVALIDCOMMAND	0x46
#define SLCD_BACKLIGHT_ON	0x81
#define SLCD_BACKLIGHT_OFF	0x80

class SerialLCD : public SoftwareSerial{
public:

    SerialLCD(uint8_t, uint8_t);
    void begin();
    void clear();
    void home();

    void noDisplay();
    void display();
    void noBlink();
    void blink();
    void noCursor();
    void cursor();
    void scrollDisplayLeft();
    void scrollDisplayRight();
    void leftToRight();
    void rightToLeft();
    void autoscroll();
    void noAutoscroll();

    void setCursor(uint8_t, uint8_t);
    void noPower(void);
    void Power(void);
    void noBacklight(void);
    void backlight(void);
    void print(uint8_t b);
    void print(const char[]);
    void print(unsigned long n, uint8_t base);
	void print(float data,uint8_t num);
};

#endif
