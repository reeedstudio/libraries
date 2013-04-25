/*
  BeaconDrive.h
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2013-2-19
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

#ifndef __BEACONDRIVE_H__
#define __BEACONDRIVE_H__

#include "BeaconDriveDfs.h"
class BeaconDrive{

public:
    bool flgRLed;                           // state of red led, 1: on 0: off
    bool flgGLed;                           // state of green led
    unsigned long rLedCnt;               	// red led count
    unsigned long gLedCnt;                 	// green led count
    
public:
    void init();
    void sysPowerOff();                     // power off
    void sysPowerOn();                      // power on
    void grovePowerOn();                    // turn on all grove
    void grovePowerOff();                   // turn off all grove
    void rfBeePowerOn();                    // turn on rfBee
    void rfBeePowerOff();                   // turn off rfBee
    void beepOn();                          // beep on
    void beepOff();                         // beep off   
    void useLed(unsigned char ucLed);       // light led
    void ledIsr();                          // led isr, you have to put this in a 1ms timer server function

#ifdef __ATOM_NODE

    void pwrDown(unsigned long tSleep);     // power down mode
    void wakeUp();                          // wake up

#endif

    void setLedShine(unsigned char color, unsigned long time);    // set led shine

};

extern BeaconDrive BcnDrive;

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
