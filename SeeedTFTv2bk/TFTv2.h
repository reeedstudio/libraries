/*
 2012 Copyright (c) Seeed Technology Inc.

 Authors: Albert.Miao & Loovee.

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
#ifndef TFTv2_h
#define TFTv2_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <avr/pgmspace.h>

#include <SPI.h>

//Basic Colors
#define RED		0xf800
#define GREEN	0x07e0
#define BLUE	0x001f
#define BLACK	0x0000
#define YELLOW	0xffe0
#define WHITE	0xffff

//Other Colors
#define CYAN		0x07ff	
#define BRIGHT_RED	0xf810	
#define GRAY1		0x8410  
#define GRAY2		0x4208  

//TFT resolution 240*320
#define MIN_X	0
#define MIN_Y	0
#define MAX_X	239
#define MAX_Y	319

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#define TFT_CS_LOW  {DDRE |= 0x08;PORTE &=~ 0x08;}
#define TFT_CS_HIGH {DDRE |= 0x08;PORTE |=  0x08;}
#define TFT_DC_LOW  {DDRH |= 0x08;PORTH &=~ 0x08;}
#define TFT_DC_HIGH {DDRH |= 0x08;PORTH |=  0x08;}
#define TFT_BL_OFF  {DDRH |= 0x10;PORTH &=~ 0x10;}
#define TFT_BL_ON   {DDRH |= 0x10;PORTH |=  0x10;}

#elif defined(__AVR_ATmega32U4__)

#define TFT_CS_LOW  {DDRC |= 0x40;PORTC &=~ 0x40;}
#define TFT_CS_HIGH {DDRC |= 0x40;PORTC |=  0x40;}
#define TFT_DC_LOW  {DDRD |= 0x80;PORTD &=~ 0x80;}
#define TFT_DC_HIGH {DDRD |= 0x80;PORTD |=  0x80;}
#define TFT_BL_OFF  {DDRE |= 0x40;PORTE &=~ 0x40;}
#define TFT_BL_ON   {DDRE |= 0x40;PORTE |=  0x40;}

#else
#define TFT_CS_LOW  {DDRD |= 0x20;PORTD &=~ 0x20;}
#define TFT_CS_HIGH {DDRD |= 0x20;PORTD |=  0x20;}
#define TFT_DC_LOW  {DDRD |= 0x40;PORTD &=~ 0x40;}
#define TFT_DC_HIGH {DDRD |= 0x40;PORTD |=  0x40;}
#define TFT_BL_OFF  {DDRD |= 0x80;PORTD &=~ 0x80;}
#define TFT_BL_ON   {DDRD |= 0x80;PORTD |=  0x80;}

#endif

extern unsigned char simpleFont[][8];

class TFT
{
public:
	void TFTinit (void);
	void setCol(unsigned int StartCol,unsigned int EndCol);
	void setPage(unsigned int StartPage,unsigned int EndPage);
	void setXY(unsigned int poX, unsigned int poY);
    void setPixel(unsigned int poX, unsigned int poY,unsigned int color);
	void sendCMD(unsigned char index);
	void WRITE_Package(unsigned int *data,unsigned char howmany);
	void WRITE_DATA(unsigned char data);
	void sendData(unsigned int data);
	unsigned char Read_Register(unsigned char Addr,unsigned char xParameter);
	void fillScreen(unsigned int XL,unsigned int XR,unsigned int YU,unsigned int YD,unsigned int color);
	void fillScreen(void);
	unsigned char readID(void);
	
	void drawChar(unsigned char ascii,unsigned int poX, unsigned int poY,unsigned int size, unsigned int fgcolor);
    void drawString(char *string,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor);
	void fillRectangle(unsigned int poX, unsigned int poY, unsigned int length, unsigned int width, unsigned int color);
	
	void drawLine(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int color);
    void drawVerticalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color);
    void drawHorizontalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color);
    void drawRectangle(unsigned int poX, unsigned int poY, unsigned int length,unsigned int width,unsigned int color);
	
	void drawCircle(int poX, int poY, int r,unsigned int color);
    void fillCircle(int poX, int poY, int r,unsigned int color);
	
	void drawTraingle(int poX1, int poY1, int poX2, int poY2, int poX3, int poY3, unsigned int color);
    unsigned char drawNumber(long long_num,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor);
    unsigned char drawFloat(float floatNumber,unsigned char decimal,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor);
    unsigned char drawFloat(float floatNumber,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor);

};

extern TFT Tft;

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/