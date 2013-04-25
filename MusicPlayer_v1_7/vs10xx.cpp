//  File vs10xx.cpp
//
//  For more details about the product please check http://www.seeedstudio.com/depot/
//  Copyright (c) 2012 seeed technology inc.
//  Author: Frankie.Chu
//  Version: 2.0
//  Time: June 2, 2012
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
#include <NewSPI.h>
#include "vs10xx.h"
#include "pins_config.h"
VS10XX vs1053;
void VS10XX::init()
{
  newSPI.begin();
  initIO();
  reset();
}
void VS10XX::initIO()
{
  pinMode(VS_XRESET, OUTPUT);
  pinMode(VS_DREQ, INPUT);
  pinMode(VS_XDCS, OUTPUT);
  pinMode(VS_XCS, OUTPUT);
  digitalWrite(VS_XDCS,HIGH);
  digitalWrite(VS_XCS,HIGH);
}

void VS10XX::writeRegister(unsigned char addressbyte,unsigned char highbyte,unsigned char lowbyte)
{ 
 // while (!readDREQ());
  deselectDataBus();
  selectControlBus(); 
  newSPI.transmit(VS_WRITE_COMMAND); 
  newSPI.transmit((addressbyte)); 
  newSPI.transmit((highbyte)); 
  newSPI.transmit((lowbyte));
  deselectControlBus();
}

void VS10XX::writeData(unsigned char *databuf, unsigned char datalen)
{
  selectDataBus();
  while (!readDREQ());
  while(datalen --)
  {
    newSPI.transmit(*databuf ++);
  }
  deselectDataBus();
}

unsigned int VS10XX::readRegister (unsigned char addressbyte)
{
  unsigned int result = 0;
  unsigned char highbyte = 0;
  unsigned char lowbyte = 0;
  deselectDataBus();
  selectControlBus();//XCS = 0
  newSPI.transmit(VS_READ_COMMAND); //send read command
  newSPI.transmit(addressbyte);    //send register address     
    //be careful: when read 0xff,dump it,and read agin
    //while((h = SPIGetChar())== 0xff);
  highbyte = newSPI.receive();    
  //while((l = VsReadByte())== 0xff);
  lowbyte = newSPI.receive();

  result = (((unsigned int) highbyte << 8) | lowbyte);
  deselectControlBus();
  return result;
}
// reset for vs10xx
void VS10XX::reset()
{
  putInReset();
  delay(100);//it is a must
  /* Send dummy SPI byte to initialize atmel SPI */
  ////SPIPutCharWithoutWaiting(0xFF);  
  deselectControlBus();
  deselectDataBus();
  releaseFromReset();
  while (!readDREQ());
  /* Set clock register, doubler etc. */
  writeRegister(SPI_CLOCKF, 0xc0, 0x00); 
  Serial.print("\r\nClockF:");
  Serial.println(readRegister(SPI_CLOCKF),HEX);
  /* Wait for DREQ */
  while (!readDREQ());
  softReset();//comment this, as it will be executed everytime playing a music file.

  writeRegister(SPI_WRAMADDR, 0xc0, 0x13);
  
  /* Switch on the analog parts */
  setVolume(40,40);
  //setVolume(0xff,0xff);
  //SPISetFastClock();
}
/** Soft Reset of VS10xx (Between songs) */
void VS10XX::softReset()
{
  /* Soft Reset of VS10xx */
  writeRegister (SPI_MODE, 0x08, 0x04); /* Newmode, Reset, No L1-2 */
  //Delay(1);
  while (!readDREQ()); /* wait for startup */
    //setup I2S (see page77 of the datasheet of vs1053 )
  //set GPIO0 as output
  writeRegister(SPI_WRAMADDR, 0xc0, 0x17);
  writeRegister(SPI_WRAM, 0x00, 0xf0);
  //enable I2S (MCLK enabled, 48kHz sample rate)
  writeRegister(SPI_WRAMADDR, 0xc0, 0x40);
  writeRegister(SPI_WRAM, 0x00, 0x0C);
  while (!readDREQ()); 
  //LoadUserPatch();
}
/** Set VS10xx Volume Register */
void VS10XX::setVolume(unsigned char leftchannel,unsigned char rightchannel)
{
  writeRegister(11,(leftchannel),(rightchannel));
}

void VS10XX::sendZerosToVS10xx()
{
  unsigned int i;
  selectDataBus();
  for (i=0;i < 2048;i ++)
  {
    while (!readDREQ());
    newSPI.transmit(0);
  }
  deselectDataBus();
}
/****************The end***********************************************/
