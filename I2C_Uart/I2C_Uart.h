/*
  I2C_Uart.h
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.
 
  Author:Loovee
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
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _I2C_UART_H_
#define _I2C_UART_H_

#define __print I2C_Uart
#define __println I2C_Uart_N

void I2C_Uart_Init();
void I2C_Uart(char *p);
void I2C_Uart(int n);
//void I2C_Uart(float n);

void I2C_Uart_N(int n);
//void I2C_Uart_N(float n);
void I2C_Uart_N(char *p);
void I2C_Uart_N();

//void __printff(float n);
//void __printffn(float n);

void putChar(unsigned char C);
void putString(const char *String);
unsigned char putNumber(long long_num);
unsigned char putFloat(float floatNumber);

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
