/*
  ILI9341 TFT Library.

  2011 Copyright (c) Seeed Technology Inc.

  Authors: Albert.Miao, Visweswara R (with initializtion code from TFT vendor)

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
#ifndef TFT_v2_h
#define TFT_v2_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <avr/pgmspace.h>


#define SEEEDUINO

//----------------------------------------------------------------------

#define RED            0xf800
#define GREEN          0x07e0
#define BLUE           0x001f
#define BLACK          0x0000
#define YELLOW         0xffe0
#define WHITE          0xffff
#define CYAN           0x07ff
#define PURPLE         0xf81f
#define BRIGHT_RED     0xf810
#define GRAY1          0x8410
#define GRAY2          0x4208

/*
 *  TFT resolution 240*320
 */
#define MIN_X    0
#define MIN_Y    0
#define MAX_X    240
#define MAX_Y    320

//----------------------------------------------------------------------
#define DDR_RST      DDRD
#define PORT_RST     PORTD
#define RST_BIT      0x04
#define RST_OUTPUT   {DDR_RST|=RST_BIT;}
#define RST_HIGH     {PORT_RST|=RST_BIT;}
#define RST_LOW      {PORT_RST&=~RST_BIT;}

//----------------------------------------------------------------------
#define DDR_BL      DDRD
#define PORT_BL     PORTD
#define BL_BIT      0x08
#define BL_OUTPUT   {DDR_BL |= BL_BIT;}
#define BL_INPUT    {DDR_BL &=~ BL_BIT;}
#define BL_ON       {DDR_BL &=~ BL_BIT;}
#define BL_OFF      {DDR_BL|=BL_BIT;PORT_BL&=~BL_BIT;}

//----------------------------------------------------------------------
#define DDR_CS      DDRB
#define PORT_CS     PORTB
#define CS_BIT      0x02
#define CS_OUTPUT   {DDR_CS|=CS_BIT;}
#define CS_HIGH     {PORT_CS|=CS_BIT;}
#define CS_LOW      {PORT_CS&=~CS_BIT;}

//----------------------------------------------------------------------
#define DDR_RS      DDRB
#define PORT_RS     PORTB
#define RS_BIT      0x04
#define RS_OUTPUT   {DDR_RS|=RS_BIT;}
#define RS_HIGH     {PORT_RS|=RS_BIT;}
#define RS_LOW      {PORT_RS&=~RS_BIT;}

#define DDR_DC      DDRB
#define PORT_DC     PORTB
#define DC_BIT      0x04
#define DC_OUTPUT   {DDR_DC|=DC_BIT;}
#define DC_HIGH     {PORT_DC|=DC_BIT;}
#define DC_LOW      {PORT_DC&=~DC_BIT;}

//----------------------------------------------------------------------
#define DDR_WR      DDRB
#define PORT_WR     PORTB
#define WR_BIT      0x08
#define WR_OUTPUT   {DDR_WR|=WR_BIT;}
#define WR_HIGH     {PORT_WR|=WR_BIT;}
#define WR_LOW      {PORT_WR&=~WR_BIT;}
#define WR_RISING   {PORT_WR|=WR_BIT;PORT_WR&=~WR_BIT;}

//----------------------------------------------------------------------
#define DDR_RD      DDRB
#define PORT_RD     PORTB
#define RD_BIT      0x20
#define RD_OUTPUT   {DDR_RD|=RD_BIT;}
#define RD_HIGH     {PORT_RD|=RD_BIT;}
#define RD_LOW      {PORT_RD&=~RD_BIT;}
#define RD_RISING   {PORT_RD|=RD_BIT;PORT_RD&=~RD_BIT;}

//----------------------------------------------------------------------
extern unsigned char font5x7[];

class TFT
{
    public:
    unsigned int IC_CODE;                                               /* Stores Driver IC ID (either  */
                                                                        /* SPFD5408A  or ST7781R)       */
    void sendCMD(unsigned int index);
    void sendData(unsigned int data);
    void sendByte(unsigned char data);
    unsigned int readData(void);
    void init (void);
    void init_io(void);
    void pushData(unsigned char data);
    unsigned char getData(void);
    void all_pin_input(void);
    void all_pin_output(void);
    void all_pin_low(void);
    void setCol(unsigned int StartCol,unsigned int EndCol);
    void setPage(unsigned int StartPage,unsigned int EndPage);
    void readReg(unsigned char Addr,unsigned char howMany);
    void fillScreen(unsigned int color);
    void fillScreen(void);

    void setXY(unsigned int poX, unsigned int poY);
    void setPixel(unsigned int poX, unsigned int poY,unsigned int color);

    void drawLine(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int color);
    void drawVerticalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color);
    void drawHorizontalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color);
    void drawRectangle(unsigned int poX, unsigned int poY, unsigned int length,unsigned int width,unsigned int color);
    void fillRectangle(unsigned int poX, unsigned int poY, unsigned int length, unsigned int width, unsigned int color);
    void drawCircle(int poX, int poY, int r,unsigned int color);
    void fillCircle(int poX, int poY, int r,unsigned int color);
    void drawChar(unsigned char ascii,unsigned int poX, unsigned int poY,unsigned int size, unsigned int fgcolor);
    void drawString(char *string,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor);
    void drawTraingle(int poX1, int poY1, int poX2, int poY2, int poX3, int poY3, unsigned int color);
    unsigned char drawNumber(long long_num,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor);
    unsigned char drawFloat(float floatNumber,unsigned char decimal,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor);
    unsigned char drawFloat(float floatNumber,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor);

    private:
};

extern TFT Tft;

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

