/*
  LightCom.cpp
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
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-
1301  USA
*/
#include <Arduino.h>
#include <I2C_Uart.h>
#include <LightCom.h>

#define D_DATALEN   0
#define D_DATA      1

/*********************************************************************************************************
** Function name: init
** Descriptions:  init pin
*********************************************************************************************************/
void LightCom::init(unsigned char pinRev)
{
    pin     = pinRev;
    time    = 0;
    state   = STATE_IDLE;
    rawLen  = 0;
    tout    = 0;
    pinMode(pin, INPUT);
#if __DEBUG
    __PRINTLN("light com init over");
#endif
}

/*********************************************************************************************************
** Function name: clear
** Descriptions:  clear data
*********************************************************************************************************/
void LightCom::clear()
{
    state   = STATE_IDLE;
    rawLen  = 0;
}

/*********************************************************************************************************
** Function name: isData
** Descriptions:  if get data?
*********************************************************************************************************/
bool LightCom::isData()
{
    if(state == STATE_STOP)
    {
#if __DEBUG
        __PRINTLN("get in is data! ");
        __PRINT("rawLen = ");__PRINTLN(rawLen);

        __PRINTLN("raw: ");
        for(int i = 0; i<rawLen; i++)
        {
            __PRINT(rawBuf[i]);
            __PRINT("\t");
        }
        __PRINTLN();
#endif
        if((rawLen)%9 == 0)
        {
#if __DEBUG
            __PRINTLN("GET GOOD DATA");
#endif
            return 1;
        }
        else
        {
            clear();
        }
    }
    return 0;
}

/*********************************************************************************************************
** Function name: TIMEISR
** Descriptions:  1m isr, put int tiemr1 or ohter
*********************************************************************************************************/
unsigned char dataInBuf = MARK;
unsigned char dataIn    = MARK;

unsigned char cntDin    = 0;

void LightCom::TIMEISR()
{
    dataInBuf   = dataIn;
    //  dataIn      = 0;
    tout ++;

    unsigned char tmpd = digitalRead(pin) ? MARK : SPACE;

    if(tmpd != dataInBuf)
    {
        cntDin++;
        if(cntDin < 2)return;
        else
        {
            dataIn = tmpd;
            cntDin = 0;
        }
    }
    else
    {
        dataIn = tmpd;
        cntDin = 0;
    }

    time++;

    if(time > 300)
    {
        time    = 0;
        rawLen  = 0;
        state   = STATE_IDLE;
    }

    if(rawLen >= RAWBUFLEN)
    {
        state = STATE_STOP;
    }

    switch(state) {
        case STATE_IDLE: // In the middle of a gap
        if(dataInBuf == SPACE && dataIn == MARK)    // falling
        {
            if(time > 85 && time < 150)
            {
                time  = 0;
                state = STATE_START;
#if __Debug
                __PRINTLN("LIGHGCOM: goto STATE_START!");
#endif
            }

        }
        if(dataIn != dataInBuf)
        {
            dataInBuf = dataIn;
            time = 0;
        }
        break;

        case STATE_START:

        if(dataIn == SPACE)
        {
            time  = 0;
            state = STATE_SPACE;
#if __DEBUG
            __PRINTLN("LIGHGCOM: begin to recvData!");
#endif
        }
        else if(time > TIMEOUT )        // time out
        {
#if __DEBUG
            __PRINTLN("LIGHGCOM: TIME OUT: STATE_START");
#endif
            time = 0;
            clear();
        }
        break;

        case STATE_MARK: // timing MARK
        if (dataIn == SPACE)
        {   
            // MARK ended, record time
            //rawBuf[rawLen++] = time;
            time  = 0;
            state = STATE_SPACE;
        }
        else
        { // SPACE
            if (time > GAP_TICKS) {
                // big SPACE, indicates gap between codes
                // Mark current code as ready for processing
                // Switch to STOP
                // Don't reset timer; keep counting space width
                state = STATE_STOP;
            }
        }
        break;
        
        case STATE_SPACE: // timing SPACE
        if (dataIn == MARK) { // SPACE just ended, record it
            rawBuf[rawLen++] = time;
            time = 0;
            state = STATE_MARK;
        }
        else if(time > TIMEOUT )        // time out
        {
#if __DEBUG
            __PRINTLN("TIME OUT: STATE_START");
#endif
            time = 0;
            clear();
        }
        break;
        
        case STATE_STOP: // waiting, measuring gap
        if (dataIn == MARK) { // reset gap timer
            time = 0;
        }
        break;
    }

}

/*********************************************************************************************************
** Function name: Recv
** Descriptions:  get data, return length of data
*********************************************************************************************************/
unsigned char LightCom::Recv(unsigned char *rDta)
{

    int count       = rawLen;
    int count_data  = 0;
    
    unsigned char revData[20];
    
    count_data = (count)/9;     // here
    
    cmpOneZero = COMONE;
    
    for(int i = 0; i<count_data; i++)
    {
        unsigned char tmp = 0;
        for(int j = 1; j<25; j++)
        {
            if(REVOK == checkCrc(&rawBuf[9*i], cmpOneZero, &tmp))
            {
#if __DEBUG
                __PRINTLN("CRC OK");
                __PRINT("tmp = ");
                __PRINTLN(tmp);
#endif
                revData[i+D_DATA] = tmp;
                break;
            }
            else
            {
                if(j%2 == 0)
                {
                    cmpOneZero += j;
                }
                else
                {
                    cmpOneZero -= j;
                }
#if __DEBUG
                __PRINTLN("**********HERE***********");
                __PRINT("cmpOneZero = ");
                __PRINTLN(cmpOneZero);
                __PRINTLN("**********HERE***********");
#endif
            }
        }
    }
    
    revData[D_DATALEN]  = count_data;
    for(int i = 0; i<revData[D_DATALEN]; i++)
    {
        rDta[i] = revData[D_DATA+i];
    }
    
#if __DEBUG
    __PRINT("\r\n*************************************************************\r\n");
    __PRINT("data_len = ");
	__PRINTLN(revData[D_DATALEN]);
    for(int i = 0; i<revData[D_DATALEN]; i++)
    {
        __PRINT(revData[D_DATA+i]);__PRINT("\t");
    }
    __PRINT("\r\n*************************************************************\r\n");
#endif
    clear(); // Receive the next value
    return revData[D_DATALEN];
}

/*********************************************************************************************************
** Function name: Partition
** Descriptions:  just use for quickSort
*********************************************************************************************************/
inline unsigned char LightCom::Partition(int a,int b, unsigned char *N)
{
    int i=a,j=b;
    unsigned char tmp = 0;
    while(i<j)
    {
        while((i<j)&&(N[i]<=N[j]))
        j--;
        if(i<j)
        {
            tmp  = N[i];
            N[i] = N[j];
            N[j] = tmp;
            i++;
        }
        while((i<j) && (N[i] <= N[j]))
        i++;
        if(i<j)
        {
            tmp  = N[i];
            N[i] = N[j];
            N[j] = tmp;
            j--;
        }
    }
    return i;
}

/*********************************************************************************************************
** Function name: quickSort
** Descriptions:  quicksort algorithm
*********************************************************************************************************/
void LightCom::quickSort(int a,int b, unsigned char *N)
{
    int pivot;
    if(a<b)
    {
        pivot = Partition(a, b, N);
        quickSort(a, pivot-1, N);
        quickSort(pivot+1, b, N);
    }
}

/*********************************************************************************************************
** Function name: quickSort
** Descriptions:  quicksort algorithm
*********************************************************************************************************/
unsigned char LightCom::checkCrc(unsigned char *dta, unsigned char dCmp, unsigned char *val)
{
    unsigned char cntOne = 0;
    *val = 0;
    for(int i = 0; i<8; i++)
    {
        if(dta[i] >= dCmp)
        {
            cntOne++;
            *val |= 0x01<<(7-i);
        }
    }
    
    if(dta[8] >= dCmp)
    {
        cntOne++;
    }
    
    if(cntOne % 2 == 0)
    {
        *val = 0;
#if __DEBUG
        __PRINTLN("CRC ERR");
#endif
        return CRCERR;
    }
    return REVOK;  
 
}


LightCom LightCom1;
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
