/*
  RF_SPI.cpp - SPI library
  Copyright (c) 2008 Cam Thompson.
  Author: Cam Thompson, Micromega Corporation, <www.micromegacorp.com>
  Version: December 15, 2008

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
#include "RFBeeSpi.h"

//---------- constructor ----------------------------------------------------

RFSPI::RFSPI()
{
  // initialize the SPI pins
  pinMode(SCK_PIN, OUTPUT);
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  pinMode(SS_PIN, OUTPUT);

  // enable SPI Master, MSB, SPI mode 0, FOSC/4
  mode(0);
  
  digitalWrite(SS_PIN,HIGH);
  digitalWrite(MOSI_PIN,LOW);
  digitalWrite(SCK_PIN,HIGH);
  
}

//------------------ mode ---------------------------------------------------

void RFSPI::mode(unsigned char config)
{
  unsigned char tmp;

  // enable SPI master with configuration unsigned char specified
  SPCR = 0;
  SPCR = (config & 0x7F) | (1<<SPE) | (1<<MSTR);
  tmp = SPSR;
  tmp = SPDR;
}

//------------------ transfer -----------------------------------------------

unsigned char RFSPI::transfer(unsigned char value)
{
  unsigned char x;
  SPDR = value;
  while (!(SPSR & (1<<SPIF))) ;
  x  = SPDR;
  return x;
}


//------------------ slave Select ---------------------------------------------
unsigned char RFSPI::slaveSelect(unsigned char value)
{
  digitalWrite(SS_PIN,value);
}

//---------- preinstantiate SPI object --------------------------------------

RFSPI RF_SPI = RFSPI();
