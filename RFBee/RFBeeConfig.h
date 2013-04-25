//  Config.h Simple config library for RFbee

//  Copyright (c) 2010 Hans Klunder <hans.klunder (at) bigfoot.com>
//  Author: Hans Klunder, based on the original Rfbee v1.0 firmware by Seeedstudio
//  Version: June 18, 2010
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



#ifndef _RFBEECONFIG_H_
#define _RFBEECONFIG_H_

#include <Arduino.h>

// config layout
#define CONFIG_RFBEE_MARKER       0  // Marker
#define CONFIG_HW_VERSION         1  // Hardware version  
#define CONFIG_FW_VERSION         2  // Firmware version
#define CONFIG_DEST_ADDR          3  // Receiver address
#define CONFIG_MY_ADDR            4  // Sender address
#define CONFIG_ADDR_CHECK         5  // Address checking
#define CONFIG_TX_THRESHOLD       6  // Transmit threshold
#define CONFIG_BDINDEX            7  // Index to baudrate
#define CONFIG_PAINDEX            8  // Index to PowerAmplifier
#define CONFIG_CONFIG_ID          9 // Selected CCx configuration
#define CONFIG_OUTPUT_FORMAT     10 // output format to use
#define CONFIG_RFBEE_MODE        11 // rfBee operating mode (e.g. transmit/receive/idle)
// marker
#define CONFIG_RFBEE_MARKER_VALUE 0xAB

class CONFIG
{
  public:
    CONFIG(void);
    void reset(void);
    unsigned char get(unsigned char);
    void set(unsigned char,unsigned char);
    int initialized();
};

extern CONFIG Config;

#endif
