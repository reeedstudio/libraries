/*
  SeeedGrayOLED.h - SSD1327 Gray OLED Driver Library
  2011 Copyright (c) Seeed Technology Inc.  All right reserved.
 
  Author: Visweswara R
  
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

#ifndef SeeedGrayOLED_data_H
#define SeeedGrayOLED_data_H

// SeeedGrayOLED Instruction set addresses

#include "Arduino.h"

#define VERTICAL_MODE			01
#define HORIZONTAL_MODE			02

#define SeeedGrayOLED_Address		0x3c
#define SeeedGrayOLED_Command_Mode	0x80
#define SeeedGrayOLED_Data_Mode		0x40

#define SeeedGrayOLED_Display_Off_Cmd	0xAE
#define SeeedGrayOLED_Display_On_Cmd	0xAF

#define SeeedGrayOLED_Normal_Display_Cmd	0xA4
#define SeeedGrayOLED_Inverse_Display_Cmd	0xA7
#define SeeedGrayOLED_Activate_Scroll_Cmd	0x2F
#define SeeedGrayOLED_Dectivate_Scroll_Cmd	0x2E
#define SeeedGrayOLED_Set_ContrastLevel_Cmd	0x81

#define Scroll_Left			0x00
#define Scroll_Right			0x01

#define Scroll_2Frames			0x7
#define Scroll_3Frames			0x4
#define Scroll_4Frames			0x5
#define Scroll_5Frames			0x0
#define Scroll_25Frames			0x6
#define Scroll_64Frames			0x1
#define Scroll_128Frames		0x2
#define Scroll_256Frames		0x3


class SeeedGrayOLED {

public:

char addressingMode;

void init(void);
void setNormalDisplay();
void setInverseDisplay();

void sendCommand(unsigned char command);
void sendData(unsigned char Data);
void setGrayLevel(unsigned char grayLevel);

void setVerticalMode();
void setHorizontalMode();

void setTextXY(unsigned char Row, unsigned char Column);
void clearDisplay();
void setContrastLevel(unsigned char ContrastLevel);
void putChar(unsigned char c);
void putString(const char *String);
unsigned char putNumber(long n);
unsigned char putFloat(float floatNumber);
unsigned char putFloat(float floatNumber,unsigned char decimal);

void drawBitmap(unsigned char *bitmaparray,int bytes);

void setHorizontalScrollProperties(bool direction,unsigned char startRow, unsigned char endRow,unsigned char startColumn, unsigned char endColumn, unsigned char scrollSpeed);
void activateScroll();
void deactivateScroll();

private:

unsigned char grayH;
unsigned char grayL;


};

extern SeeedGrayOLED SeeedGrayOled;  // SeeedGrayOLED object 

#endif


