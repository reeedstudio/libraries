//  rfBeeCore.cpp core routines for the rfBee
//  see www.seeedstudio.com for details and ordering rfBee hardware.

//  Copyright (c) 2010 Hans Klunder <hans.klunder (at) bigfoot.com>
//  Author: Hans Klunder, based on the original Rfbee v1.0 firmware by Seeedstudio
//  Version: July 14, 2010
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

#include "RFBeeCore.h"

// send data via RF
void transmitData(unsigned char *txData,unsigned char len, unsigned char srcAddress, unsigned char destAddress)
{
    unsigned char stat;

  CCx.Write(CCx_TXFIFO,len+2);
  CCx.Write(CCx_TXFIFO,destAddress);
  CCx.Write(CCx_TXFIFO,srcAddress);
  CCx.WriteBurst(CCx_TXFIFO,txData, len); // write len bytes of the serialData buffer into the CCx txfifo
  CCx.Strobe(CCx_STX);
  while(1){
    unsigned char size;
    stat=CCx.Read(CCx_TXBYTES, &size);
    if( 0 == size ){
      break;
    }
    else{
      CCx.Strobe(CCx_STX);
    }
  }

}

// receive data via RF, rxData must be at least CCx_PACKT_LEN bytes long
int receiveData(unsigned char *rxData, unsigned char *len, unsigned char *srcAddress, unsigned char *destAddress, unsigned char *rssi , unsigned char *lqi){

    unsigned char stat;

  stat=CCx.Read(CCx_RXFIFO,len);
  CCx.Read(CCx_RXFIFO,destAddress);
  CCx.Read(CCx_RXFIFO,srcAddress);
  *len -= 2;  // discard address bytes from payloadLen 
  CCx.ReadBurst(CCx_RXFIFO, rxData,*len);
  CCx.Read(CCx_RXFIFO,rssi);
  *rssi=CCx.RSSIdecode(*rssi);
  stat=CCx.Read(CCx_RXFIFO,lqi);
  // check checksum ok
  if ((*lqi & 0x80)==0){
    return NOTHING;
  }
  *lqi=*lqi & 0x7F; // strip off the CRC bit

  // handle potential RX overflows by flushing the RF FIFO as described in section 10.1 of the CC 1100 datasheet
  if ((stat & 0xF0) == 0x60){ //Modified by Icing. When overflows, STATE[2:0] = 110
    CCx.Strobe(CCx_SFRX); // flush the RX buffer
    return ERR;
  }
  return OK;
}