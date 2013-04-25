/*
 Rainbowduino v3.0 Library. 
 
 2011 Copyright (c) Seeed Technology Inc.
 
 Authors: Albert.Miao, Visweswara R

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

/*
This library heavily makes use of line, circle and other shape drawing algorithms and code presented
in paper "The Beauty of Bresenham's Algorithm" by Alois Zingl, Vienna, Austria. The original author
has kindly released these example programs with no copyright.
*/

#ifndef RAINBOWDUINO_h
#define RAINBOWDUINO_h

#include <WProgram.h>
#include <avr/pgmspace.h>


// MY9221 driver interfaces 

#define DDR_Data  DDRB
#define DDR_Clk   DDRB

#define PORT_Data PORTB
#define PORT_Clk  PORTB

#define BIT_Data  0x01
#define BIT_Clk   0x02

// 3-to-8 Decoder Lines

#define DDR_Lines  DDRD
#define PORT_Lines PORTD
#define BIT_Lines  0xF0
#define switchOffDrive	{PORT_Lines &=~ 0x80;}

#define CmdMode 0b0000000000000000

extern unsigned char simpleFont[][8];

class Rainbowduino
{
public:
    void init();
    void init_timer1(void);
    void send16bitData(unsigned int data);
    void latchData(void);
    void switchOnDrive(unsigned char line);
    void clearDisplay(void); //Is not made private method as used in ISR. Use this like a private method.
    void blankDisplay(void);
    void setPixelXY(unsigned char x, unsigned char y, uint32_t colorRGB /*24-Bit RGB Color*/);
    void setPixelXY(unsigned char x, unsigned char y, unsigned char  colorR,  unsigned char colorG, unsigned char colorB);
    void setPixelZXY(unsigned char z, unsigned char x, unsigned char y,  uint32_t  /*24-bit RGB Color*/);
    void setPixelZXY(unsigned char z, unsigned char x, unsigned char y, unsigned char  colorR,  unsigned char colorG, unsigned char colorB);
    void setPixelXY(unsigned char start, unsigned char end, uint32_t *colorRGB);
    void drawCircle(int poX, int poY, int r, uint32_t colorRGB /*24-bit RGB Color*/);
    void fillCircle(int poX, int poY, int r, uint32_t colorRGB /*24-bit RGB Color*/);
    void drawLine(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1, uint32_t colorRGB /*24-bit RGB Color*/);
    void drawVerticalLine(unsigned int poX, unsigned int poY,unsigned int length, uint32_t colorRGB /*24-bit RGB Color*/);
    void drawHorizontalLine(unsigned int poX, unsigned int poY,unsigned int length, uint32_t colorRGB /*24-bit RGB Color*/);
    void drawRectangle(unsigned int poX, unsigned int poY, unsigned int length,unsigned int width, uint32_t colorRGB /*24-bit RGB Color*/);
    void fillRectangle(unsigned int poX, unsigned int poY, unsigned int length, unsigned int width, uint32_t colorRGB /*24-bit RGB Color*/);
    void drawChar(unsigned char ascii,unsigned int poX, unsigned int poY, unsigned int colorRGB /*24-bit RGB Color*/);

private:

};

extern Rainbowduino Rb;

#endif
