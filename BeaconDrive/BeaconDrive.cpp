/*
  BeaconDrive.cpp
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
#include <Arduino.h>


#include "BeaconDrive.h"

#ifdef __ATOM_NODE

#include <Sleep_n0m1.h>

Sleep sleep;
#endif
/*********************************************************************************************************
** Function name:           grovePowerOn
** Descriptions:            grove power on
*********************************************************************************************************/
void BeaconDrive::init()
{
	
#ifdef __ATOM_NODE
		pinMode(PINSYSPWR, OUTPUT);
		sysPowerOn();
		gLedCnt 		= 0;   
		flgGLed 		= 0;
#endif
		rLedCnt 		= 0;
		flgRLed 		= 0;
	
		pinMode(PINSYSBUTT, INPUT);
		digitalWrite(PINSYSBUTT, HIGH);
	
#ifdef __ATOM_NODE
		pinMode(PINBEEP, OUTPUT);
		digitalWrite(PINBEEP, LOW);
	
		pinMode(PINPWRLOW, OUTPUT);
		digitalWrite(PINPWRLOW, HIGH);
	
		pinMode(PINSYSBUTT, OUTPUT);
		digitalWrite(PINSYSBUTT, HIGH);
	
		pinMode(PINRFBEEPWR, OUTPUT);
		digitalWrite(PINRFBEEPWR, LOW);
	
		pinMode(PINBATLOW, OUTPUT);
		digitalWrite(PINBATLOW, LOW);
	
		pinMode(PINGROVEPWR, OUTPUT);					// power on
		grovePowerOn();
	
		pinMode(PINLEDGREEN, OUTPUT);
		digitalWrite(PINLEDGREEN, LOW);
#endif
		
		pinMode(PINLEDRED, OUTPUT);
		digitalWrite(PINLEDRED, LOW);
}

/*********************************************************************************************************
** Function name:           grovePowerOn
** Descriptions:            grove power on
*********************************************************************************************************/
void BeaconDrive::grovePowerOn()
{
#ifdef __ATOM_NODE

    digitalWrite(PINGROVEPWR, LOW);
#endif
}

/*********************************************************************************************************
** Function name:           grovePowerOff
** Descriptions:            grove power off
*********************************************************************************************************/
void BeaconDrive::grovePowerOff()
{
#ifdef __ATOM_NODE

    digitalWrite(PINGROVEPWR, HIGH);
#endif
}

/*********************************************************************************************************
** Function name:           sysPowerOn
** Descriptions:            system power on
*********************************************************************************************************/
void BeaconDrive::sysPowerOn()
{
#ifdef __ATOM_NODE
    digitalWrite(PINSYSPWR, HIGH);
#endif
}

/*********************************************************************************************************
** Function name:           sysPowerOff
** Descriptions:            system power off
*********************************************************************************************************/
void BeaconDrive::sysPowerOff()
{
#ifdef __ATOM_NODE

    digitalWrite(PINSYSPWR, LOW);
#endif
}

/*********************************************************************************************************
** Function name:           useLed
** Descriptions:            light led
*********************************************************************************************************/
void BeaconDrive::useLed(unsigned char ucLed)
{
    unsigned char d1 = ucLed&0x01? HIGH: LOW;
    unsigned char d2 = ucLed&0x02? HIGH: LOW;
    digitalWrite(PINLEDGREEN, d1);
    digitalWrite(PINLEDRED, d2);
}

/*********************************************************************************************************
** Function name:           ledIsr
** Descriptions:            light led
*********************************************************************************************************/
void BeaconDrive::ledIsr()
{

    rLedCnt = rLedCnt > 0 ? rLedCnt-1 : rLedCnt;
    gLedCnt = gLedCnt > 0 ? gLedCnt-1 : gLedCnt;

    if(flgRLed && !rLedCnt)
    {
        flgRLed = 0;
        digitalWrite(PINLEDRED, LOW);

    }

    if(flgGLed && !gLedCnt)
    {
        flgGLed = 0;
        digitalWrite(PINLEDGREEN, LOW);
		//digitalWrite(A3, LOW);
    }

}


/*********************************************************************************************************
** Function name:           setLedShine
** Descriptions:            color: 1:LEDCOLORGREEN 2:LEDCOLORRED
*********************************************************************************************************/
void BeaconDrive::setLedShine(unsigned char color, unsigned long time)
{
#ifdef __ATOM_NODE

    if(color == LEDCOLORGREEN)
    {
        flgGLed         = 1;
        gLedCnt         = time;
        digitalWrite(PINLEDGREEN, HIGH);
		//digitalWrite(A3, HIGH);
		
    }
    else if(color == LEDCOLORRED)
    {
        flgRLed         = 1;
        rLedCnt         = time;
        digitalWrite(PINLEDRED, HIGH);
    }
#else
	rLedCnt = time;
	flgRLed 		= 1;
	digitalWrite(PINLEDRED, HIGH);

#endif

}

/*********************************************************************************************************
** Function name:           rfBeePowerOn
** Descriptions:            turn on rfBee
*********************************************************************************************************/
void BeaconDrive::rfBeePowerOn()
{
#ifdef __ATOM_NODE

    digitalWrite(PINRFBEEPWR, LOW);
#endif
}

/*********************************************************************************************************
** Function name:           rfBeePowerOff
** Descriptions:            turn off rfBee
*********************************************************************************************************/
void BeaconDrive::rfBeePowerOff()
{
#ifdef __ATOM_NODE

    digitalWrite(PINRFBEEPWR, HIGH);
#endif
}

/*********************************************************************************************************
** Function name:           rfBeePowerOff
** Descriptions:            turn off rfBee
*********************************************************************************************************/
void BeaconDrive::beepOn()
{
#ifdef __ATOM_NODE

    digitalWrite(PINBEEP, HIGH);
#endif
}

/*********************************************************************************************************
** Function name:           rfBeePowerOff
** Descriptions:            turn off rfBee
*********************************************************************************************************/
void BeaconDrive::beepOff()
{
#ifdef __ATOM_NODE
    digitalWrite(PINBEEP, LOW);
#endif
}

/*********************************************************************************************************
** Function name:           pwrDown
** Descriptions:            power down mode
*********************************************************************************************************/

#ifdef __ATOM_NODE
void BeaconDrive::pwrDown(unsigned long tSleep)
{
    power_usart0_disable();
    power_spi_disable();
    power_twi_disable();

	power_adc_disable();
    power_timer2_disable();
    power_timer3_disable();
    power_usart1_disable();
    power_usb_disable();
    sleep.pwrDownMode();                                //set sleep mode
    sleep.sleepDelay(tSleep);                           //sleep for: sleepTime
}

/*********************************************************************************************************
** Function name:           wakeUp
** Descriptions:            wakeUp
*********************************************************************************************************/
void BeaconDrive::wakeUp()
{

    power_usart0_enable();
    power_spi_enable();
    power_twi_enable();

    power_adc_enable();
    power_timer2_enable();
    power_timer3_enable();
    power_usart1_enable();
    power_usb_enable();

}
#endif

BeaconDrive BcnDrive;
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
