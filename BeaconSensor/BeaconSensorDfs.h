/*
  BeaconSensorDfs.h
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2012-12-2
  
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

#ifndef __BEACONSENSORDFS_H__
#define __BEACONSENSORDFS_H__

#include <Arduino.h>

#if defined( __AVR_ATmega32U4__)

#define IOSENSOR0                   A0
#define IOSENSOR1                   A1
#else
#define IOSENSOR0                   A2
#define IOSENSOR1                   A3

#endif
#define BUTTON                      1
#define TILTSWITCH                  2
#define LINEFINDER                  3
#define PIRMOTIONSENSOR             4
#define INFRAREDREFLECTIVESENSOR    5
#define MAGNETICSWITCH              6
#define TOUCHSENSOR                 7
#define IRDISTANCEINTERRUPTER       8
#define HALLSENSOR                  9
#define SWITCH                      10
#define COLLISIONSENSOR             11

#define MOISTURESENSOR              12
#define LIGHTSENSOR                 13
#define ROTARYANGLESENSOR           14
#define TEMPERATURESENSOR           44
#define WATERSENSOR                 45
#define INFRAREDPROXIMITYSENSOR     46
#define INFRAREDTEMPERATURESENSOR   47
#define SLIDEPOTENTIOMETER          48
#define AIRQUALITYSENSOR            49
#define ELECTRICITYSENSOR           50
#define ALCOHOLSENSOR               51
#define SOUNDSENSOR                 53

#define SENSORANALOGTOP             80

#define DIGITALLIGHTSENSOR          81
#define BAROMETERSENSOR             82
#define TEMPANDHUMI_TEMP			102
#define TEMPANDHUMI_HUMI			103


#define INFRAREDRECEIVER            110

#define BAROMETERADDRESS            0x77

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
