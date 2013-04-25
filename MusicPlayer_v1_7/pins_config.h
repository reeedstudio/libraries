//  File pins_config.h
//  Headers for configurating IOs to Music Shield
//  For more details about the product please check http://www.seeedstudio.com/depot/

//  Copyright (c) 2012 seeed technology inc.
//  Author: Frankie.Chu
//  Version: 2.0
//  Time: June 4, 2012
//  Changing records:
//    
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef PINS_CONFIG_H
#define PINS_CONFIG_H

#include <avr/io.h>
#include <arduino.h>
////////////////////For some control pins//////////////////////////
#define ROTARY_ANGLE_SENSOR A4
#define BUTTON              7
////////////////////////for MMC////////////////////////////////////
#define SD_CS_PIN 10  //SD card select pin linked to pin10 of MCU

///////////////////////for vs10xx///////////////////////////////////
#define VS_XRESET A0
#define VS_DREQ   A1
#define VS_XDCS   A2
#define VS_XCS    A3

/////////////////////for keys//////////////////////////////////////
#define KEY_VD   7
#define KEY_BK   6
#define KEY_PS   5
#define KEY_NT   4
#define KEY_VU   3

//////////////////for Green LED///////////////////////////////////////
#define GREEN_LED 	8
#define GREEN_LED_ON()	digitalWrite(GREEN_LED, HIGH)
#define GREEN_LED_OFF()	digitalWrite(GREEN_LED, LOW)

/********initialization for LED and keys******************************/
inline void initIOForLED()
{
  pinMode(GREEN_LED, OUTPUT);
  GREEN_LED_OFF(); 
}

inline void initIOForKeys()
{
  //keys input
  pinMode(KEY_VD, INPUT);
  pinMode(KEY_BK, INPUT);
  pinMode(KEY_PS, INPUT);
  pinMode(KEY_NT, INPUT);
  pinMode(KEY_VU, INPUT);
  //internal pull-up
  digitalWrite(KEY_VD, HIGH);
  digitalWrite(KEY_BK, HIGH);
  digitalWrite(KEY_PS, HIGH);
  digitalWrite(KEY_NT, HIGH);
  digitalWrite(KEY_VU, HIGH);
}


#endif

