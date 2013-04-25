/*
  LightComFrank.h
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2012-11-10
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
#ifndef __LIGHTCOM__
#define __LIGHTCOM__

#define __DEBUG         1
#define __BEACON        0

#define COMONE          40

#define ANALOGHIGH      350
#define ANALOGLOW       250

#define RAWBUFLEN       150
#define ISRTIME         1000
#define TIMEOUT         150         // 500

#define STATE_IDLE      2
#define STATE_START     3
#define STATE_MARK      4
#define STATE_SPACE     5
#define STATE_STOP      6

#define MARK            1
#define SPACE           0

#define GAP_TICKS       120  // 90
#define GAP_START       45   // 45

#define REVOK           0
#define TIMEOVER        1
#define CRCERR          2

#if __BEACON
#define __PRINT     __print
#define __PRINTLN   __println

#else

#define __PRINT     Serial.print
#define __PRINTLN   Serial.println
#endif

class LightCom
{
    public:
    unsigned int time;
    unsigned char pin;
    unsigned char state;
    unsigned char rawBuf[RAWBUFLEN];
    unsigned int rawLen;
    unsigned int tout;
    
    unsigned char cmpOneZero;
    
    private:
    
    void clear();
    inline unsigned char Partition(int a,int b, unsigned char *N);
    void quickSort(int a,int b, unsigned char *N);    // quick sort
    unsigned char checkCrc(unsigned char *dta, unsigned char dCmp, unsigned char *val);       // check crc & return the byte  
    
    public:
    
    void init(unsigned char pinRev);
    bool isData();                                  // if get good data
    void TIMEISR();                                 // timer isr
    unsigned char Recv(unsigned char *rDta);        // get data: return byte of data, data will store in revData

};

extern LightCom LightCom1;

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
