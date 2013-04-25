/*
  BeaconDriveDfs.h
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

#ifndef __BEACONDRIVEDFS_H__
#define __BEACONDRIVEDFS_H__

#if defined( __AVR_ATmega32U4__)
#define	__ATOM_NODE
#else
#define __CLOUD_DRAGROVE
#endif

#define PINPWRLOW                   5
#define PINGROVEPWR                 7                   // grove power pin
#define PINSYSPWR                   8                   // system power pin


#ifdef	__ATOM_NODE
#define PINSYSBUTT                  5                   // system switch
#define PINLEDGREEN                 11                  // green led pin
#define PINLEDRED                   13                  // red led pin

#else
#define PINSYSBUTT                  A5                  // system switch
#define PINLEDGREEN                 13                  // green led pin
#define PINLEDRED                   13                  // red led pin


#endif
#define PINRFBEEPWR                 A4                  // RFBEE power pin
#define PINBATLOW                   6                   // battery low led
#define PINBEEP                     A3                  // buzzer


// led color
#define     LEDCOLOROFF             0x00                // led off
#define     LEDCOLORGREEN           0x01                // green
#define     LEDCOLORRED             0x02                // red

#define power_adc_enable()          (PRR0 &= (uint8_t)~(1 << PRADC))
#define power_adc_disable()         (PRR0 |= (uint8_t)(1 << PRADC))

#define power_usart0_enable()       (PRR0 &= (uint8_t)~(1 << PRUSART0))
#define power_usart0_disable()      (PRR0 |= (uint8_t)(1 << PRUSART0))

#define power_spi_enable()          (PRR0 &= (uint8_t)~(1 << PRSPI))
#define power_spi_disable()         (PRR0 |= (uint8_t)(1 << PRSPI))

#define power_twi_enable()          (PRR0 &= (uint8_t)~(1 << PRTWI))
#define power_twi_disable()         (PRR0 |= (uint8_t)(1 << PRTWI))

#define power_timer0_enable()       (PRR0 &= (uint8_t)~(1 << PRTIM0))
#define power_timer0_disable()      (PRR0 |= (uint8_t)(1 << PRTIM0))

#if defined(__AVR_ATmega32U4__)
#define power_timer1_enable()       (PRR0 &= (uint8_t)~(1 << PRTIM1))
#define power_timer1_disable()      (PRR0 |= (uint8_t)(1 << PRTIM1))

#define power_timer2_enable()       (PRR0 &= (uint8_t)~(1 << PRTIM2))
#define power_timer2_disable()      (PRR0 |= (uint8_t)(1 << PRTIM2))

#define power_timer3_enable()       (PRR1 &= (uint8_t)~(1 << PRTIM3))
#define power_timer3_disable()      (PRR1 |= (uint8_t)(1 << PRTIM3))

#define power_usart1_enable()       (PRR1 &= (uint8_t)~(1 << PRUSART1))
#define power_usart1_disable()      (PRR1 |= (uint8_t)(1 << PRUSART1))

#define power_usb_enable()          (PRR1 &= (uint8_t)~(1 << PRUSB))
#define power_usb_disable()         (PRR1 |= (uint8_t)(1 << PRUSB))

#endif
#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
