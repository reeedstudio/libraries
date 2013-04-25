/*
  RFBeeSendRev.h
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:
  Hardware: Loovee

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
#ifndef _RFBEESENDREV_H_
#define _RFBEESENDREV_H_

// delay
class rfbeeSendRev{

    private:
    
    public:
    
    void init();
    bool isDta();
    void sendDta(int len, unsigned char *dta);
	
};

extern rfbeeSendRev RFBEE;

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
