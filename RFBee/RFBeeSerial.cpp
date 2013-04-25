//  rfBeeSerial.pde serial interface to rfBee
//  see www.seeedstudio.com for details and ordering rfBee hardware.

//  Copyright (c) 2010 Hans Klunder <hans.klunder (at) bigfoot.com>
//  Author: Hans Klunder, based on the original Rfbee v1.0 firmware by Seeedstudio
//  Version: August 24, 2010
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
#include "RFBeeSerial.h"

unsigned char errNo;

int setMyAddress(){

  CCx.Write(CCx_ADDR,Config.get(CONFIG_MY_ADDR)); 
  return OK;
} 

int setAddressCheck(){
  CCx.Write(CCx_PKTCTRL1, Config.get(CONFIG_ADDR_CHECK) | 0x04);
  return OK;
} 
 
int setPowerAmplifier(){

  CCx.setPA(Config.get(CONFIG_CONFIG_ID), Config.get(CONFIG_PAINDEX));
  return OK;
}

int setCCxConfig(){
  // load the appropriate config table
  unsigned char cfg=Config.get(CONFIG_CONFIG_ID);
  CCx.Setup(cfg);  
  setMyAddress();
  setAddressCheck();
  setPowerAmplifier();
  setRFBeeMode();
  return OK;
}

int setRFBeeMode(){

  // CCx_MCSM1 is configured to have TX and RX return to proper state on completion or timeout
  switch (Config.get(CONFIG_RFBEE_MODE))
  {
  case TRANSMIT_MODE:
    CCx.Strobe(CCx_SIDLE);
    delay(1);
    CCx.Write(CCx_MCSM1 ,   0x00 );//TXOFF_MODE->stay in IDLE
    CCx.Strobe(CCx_SFTX);
    break;
  case RECEIVE_MODE:
    CCx.Strobe(CCx_SIDLE);
    delay(1);
    CCx.Write(CCx_MCSM1 ,   0x0C );//RXOFF_MODE->stay in RX
    CCx.Strobe(CCx_SFRX);
    CCx.Strobe(CCx_SRX);
    break;
  case TRANSCEIVE_MODE:
    CCx.Strobe(CCx_SIDLE);
    delay(1);
    CCx.Write(CCx_MCSM1 ,   0x0F );//RXOFF_MODE and TXOFF_MODE stay in RX
    CCx.Strobe(CCx_SFTX);
    CCx.Strobe(CCx_SFRX);
    CCx.Strobe(CCx_SRX);
    break;
  case LOWPOWER_MODE:
    CCx.Strobe(CCx_SIDLE);
    break;  
  default:		
    break;
  }
  return OK;
}