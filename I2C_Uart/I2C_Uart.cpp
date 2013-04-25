/*
  I2C_Uart.cpp
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author: Loovee
  2012-11-21

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
#include <Wire.h>
#include <Arduino.h>
#include "I2C_Uart.h"

#define BUFLEN      200

void I2C_Uart_Init()
{
    Wire.begin();
}

void I2C_Uart(char *p)
{
    int cnt=0;
    char tmp[33];
    for(int i = 0; *(p+i); i++)
    {
        cnt++;
    }
    cnt++;

    int cnt_t = cnt/32;
    for(int i = 0; i<cnt_t; i++)
    {
        memcpy(tmp, &p[32*i], 32);
        tmp[33] = '\0';
        Wire.beginTransmission(4);          // transmit to device #4
        Wire.write(tmp);                    // sends one byte
        Wire.endTransmission();             // stop transmitting
    }
    memcpy(tmp, &p[32*cnt_t], cnt%32);
    tmp[cnt] = '\0';
    Wire.beginTransmission(4);              // transmit to device #4
    Wire.write(tmp);                        // sends one byte
    Wire.endTransmission();                 // stop transmitting


    delay(20);
}

void I2C_Uart(int n)
{
    char s[10];
    sprintf(s, "%d", n);
    I2C_Uart(s);

}

void I2C_Uart_N(int n)
{
    I2C_Uart(n);
    Wire.beginTransmission(4);              // transmit to device #4
    Wire.write('\r');
    Wire.write('\n');                        // sends one byte
    Wire.endTransmission();                 // stop transmitting

}

void I2C_Uart_N(char *p)
{
    I2C_Uart(p);
    Wire.beginTransmission(4);              // transmit to device #4
    Wire.write('\r');
    Wire.write('\n');                        // sends one byte
    Wire.endTransmission();                 // stop transmitting
}

void I2C_Uart_N()
{
    Wire.beginTransmission(4);              // transmit to device #4
    Wire.write('\r');
    Wire.write('\n');                        // sends one byte
    Wire.endTransmission();                 // stop transmitting

}

#if 0
void __printff(float n)
{



    n = n*10000.0;

    //I2C_Uart_N (n);
    n = abs(n);
    unsigned long a = abs(n);

    if(n<0)
    {
        I2C_Uart ("-");
    }

    I2C_Uart(a/10000);
    Wire.beginTransmission(4);              // transmit to device #4
    Wire.write('.');                        // sends one byte
    Wire.endTransmission();                 // stop transmitting

    a = (unsigned long)a%10000;



    if(a < 10)
    {
        I2C_Uart ("000");
    }
    else if(a < 100)
    {
        I2C_Uart ("00");
    }
    else if(a<1000)
    {
        I2C_Uart ("0");
    }


    I2C_Uart(a);
}

void __printffn(float n)
{
    __printff(n);
    I2C_Uart_N();
}

#else

void putChar(unsigned char c)
{
    Wire.beginTransmission(4);              // transmit to device #4
    Wire.write(c);                        // sends one byte
    Wire.endTransmission();                 // stop transmitting
}

void putString(const char *String)
{
    unsigned char i=0;
    while(String[i])
    {
        putChar(String[i]);     
        i++;
    }
}

unsigned char putNumber(long long_num)
{
  unsigned char char_buffer[10]="";
  unsigned char i = 0;
  unsigned char f = 0;

  if (long_num < 0) 
  {
    f=1;
    putChar('-');
    long_num = -long_num;
  } 
  else if (long_num == 0) 
  {
    f=1;
    putChar('0');
    return f;
  } 

  while (long_num > 0) 
  {
    char_buffer[i++] = long_num % 10;
    long_num /= 10;
  }

  f=f+i;
  for(; i > 0; i--)
  {
    putChar('0'+ char_buffer[i - 1]);
  }
  delay(10);
  return f;

}

unsigned char putFloat(float floatNumber)
{
    unsigned char decimal = 5;
    unsigned int temp=0;
    float decy=0.0;
    float rounding = 0.5;
    unsigned char f=0;
    if(floatNumber<0.0)
    {
        putString("-");
        floatNumber = -floatNumber;
        f +=1;
    }
    for (unsigned char i=0; i<decimal; ++i)
    {
        rounding /= 10.0;
    }
    floatNumber += rounding;

    temp = floatNumber;
    f += putNumber(temp);
    if(decimal>0)
    {
        putChar('.');
        f +=1;
    }
    decy = floatNumber-temp;//decimal part,
    for(unsigned char i=0;i<decimal;i++)//4
    {
        decy *=10;// for the next decimal
        temp = decy;//get the decimal
        putNumber(temp);
        decy -= temp;
    }
    f +=decimal;
    return f;
}

#endif



/***********************************************************************
**********************************
  END FILE
************************************************************************
*********************************/
