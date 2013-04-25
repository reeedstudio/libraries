//  Config.pde Simple config library for rfBee
//  see www.seeedstudio.com for details and ordering rfBee hardware.

//  Copyright (c) 2010 Hans Klunder <hans.klunder (at) bigfoot.com>
//  Author: Hans Klunder, based on the original Rfbee v1.0 firmware by Seeedstudio
//  Version: August 18, 2010
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


#include "RFBeeConfig.h"
#include "RFBeeGlobals.h"
#include <EEPROM.h>

//---------- constructor ----------------------------------------------------

CONFIG::CONFIG(){}

//------------------ reset ---------------------------------------------------

void CONFIG::reset()
{
    EEPROM.write(CONFIG_RFBEE_MARKER, CONFIG_RFBEE_MARKER_VALUE );
    EEPROM.write(CONFIG_FW_VERSION,FIRMWAREVERSION);
    EEPROM.write(CONFIG_DEST_ADDR,0);
    EEPROM.write(CONFIG_MY_ADDR,0);
    EEPROM.write(CONFIG_ADDR_CHECK,0x00);
    EEPROM.write(CONFIG_TX_THRESHOLD,0x01);
    EEPROM.write(CONFIG_BDINDEX,0x00);
    EEPROM.write(CONFIG_PAINDEX,0x07);
    EEPROM.write(CONFIG_CONFIG_ID,0x00);
    if (EEPROM.read(CONFIG_HW_VERSION) < 10)
          EEPROM.write(CONFIG_HW_VERSION,10);  // dirty hack to ensure rfBee's without a hardware version get their hardware version set to 1.0
    EEPROM.write(CONFIG_OUTPUT_FORMAT,0x00);   
    EEPROM.write(CONFIG_RFBEE_MODE,0x00); 
}

//------------------ get -----------------------------------------------

unsigned char CONFIG::get(unsigned char idx)
{
  return(EEPROM.read(idx));
}


//------------------ get Select ---------------------------------------------
void CONFIG::set(unsigned char idx, unsigned char value)
{
  EEPROM.write(idx,value);
}

int CONFIG::initialized()
{
 if (( EEPROM.read(CONFIG_RFBEE_MARKER) == CONFIG_RFBEE_MARKER_VALUE) &&
     ( EEPROM.read(CONFIG_FW_VERSION) == FIRMWAREVERSION))
     return OK;
 return ERR; 
}
//---------- preinstantiate Config object --------------------------------------

CONFIG Config = CONFIG();
