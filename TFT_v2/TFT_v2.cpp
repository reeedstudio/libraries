/*
  ILI9341 TFT Library.

  2012 Copyright (c) Seeed Technology Inc.

  Authors: Albert.Miao (with initializtion code from TFT vendor)

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

#include "TFT_v2.h"

#define FONT_SPACE 6

void TFT::sendCMD(unsigned int index)
{
    DC_LOW;
    RD_HIGH;
    CS_LOW;

    WR_LOW;
    pushData(index);
    WR_HIGH;
    CS_HIGH;
}

void TFT::sendData(unsigned int data)
{
    DC_HIGH;
    RD_HIGH;
    CS_LOW;

    WR_LOW;
    pushData(data>>8);
    WR_HIGH;

    WR_LOW;
    pushData(data);
    WR_HIGH;

    CS_HIGH;

}

void TFT::sendByte(unsigned char data)
{

    DC_HIGH;
    RD_HIGH;
    CS_LOW;

    WR_LOW;
    pushData(data);
    WR_HIGH;

    CS_HIGH;
}

unsigned int TFT::readData(void)
{
    unsigned int data=0;
    all_pin_input();
    all_pin_low();
    delay(1);

    DC_HIGH;
    WR_HIGH;
    CS_LOW;

    RD_LOW;
    RD_HIGH;
    delay(1);
    data = getData()<<8;

    RD_LOW;
    RD_HIGH;
    delay(1);
    data |= getData();

    Serial.print("0x");
    Serial.println(data,HEX);

    CS_HIGH;
    all_pin_output();
    return data;
}

//when char SPI 's attribute, such as use ts.getPoint   
void TFT::init_io(void)
{
    CS_OUTPUT;
    RD_OUTPUT;
    WR_OUTPUT;
    RS_OUTPUT;
    RST_OUTPUT;
    all_pin_output();
    all_pin_low();
}
void  TFT::init(void)
{
    IC_CODE = 0x5408;
    CS_OUTPUT;
    RD_OUTPUT;
    WR_OUTPUT;
    RS_OUTPUT;
    RST_OUTPUT;
    all_pin_output();
    all_pin_low();

    sendCMD(0x01);
    delay(50);

    sendCMD(0xCF);
    sendByte(0x00);
    sendByte(0x8B);
    sendByte(0X30);

    sendCMD(0xED);
    sendByte(0x67);
    sendByte(0x03);
    sendByte(0X12);
    sendByte(0X81);

    sendCMD(0xE8);
    sendByte(0x85);
    sendByte(0x10);
    sendByte(0x7A);

    sendCMD(0xCB);
    sendByte(0x39);
    sendByte(0x2C);
    sendByte(0x00);
    sendByte(0x34);
    sendByte(0x02);

    sendCMD(0xF7);
    sendByte(0x20);

    sendCMD(0xEA);
    sendByte(0x00);
    sendByte(0x00);

    sendCMD(0xC0);                                                      /* Power control                */
    sendByte(0x1B);                                                     /* VRH[5:0]                     */

    sendCMD(0xC1);                                                      /* Power control                */
    sendByte(0x10);                                                     /* SAP[2:0];BT[3:0]             */

    sendCMD(0xC5);                                                      /* VCM control                  */
    sendByte(0x3F);
    sendByte(0x3C);

    sendCMD(0xC7);                                                      /* VCM control2                 */
    sendByte(0XB7);

    sendCMD(0x36);                                                      /* Memory Access Control        */
    sendByte(0x08);

    sendCMD(0x3A);
    sendByte(0x55);

    sendCMD(0xB1);
    sendByte(0x00);
    sendByte(0x1B);

    sendCMD(0xB6);                                                      /* Display Function Control     */
    sendByte(0x0A);
    sendByte(0xA2);


    sendCMD(0xF2);                                                      /* 3Gamma Function Disable      */
    sendByte(0x00);

    sendCMD(0x26);                                                      /* Gamma curve selected         */
    sendByte(0x01);

    sendCMD(0xE0);                                                      /* Set Gamma                    */
    sendByte(0x0F);
    sendByte(0x2A);
    sendByte(0x28);
    sendByte(0x08);
    sendByte(0x0E);
    sendByte(0x08);
    sendByte(0x54);
    sendByte(0XA9);
    sendByte(0x43);
    sendByte(0x0A);
    sendByte(0x0F);
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);

    sendCMD(0XE1);                                                      /* Set Gamma                    */
    sendByte(0x00);
    sendByte(0x15);
    sendByte(0x17);
    sendByte(0x07);
    sendByte(0x11);
    sendByte(0x06);
    sendByte(0x2B);
    sendByte(0x56);
    sendByte(0x3C);
    sendByte(0x05);
    sendByte(0x10);
    sendByte(0x0F);
    sendByte(0x3F);
    sendByte(0x3F);
    sendByte(0x0F);

    sendCMD(0x11);                                                      /* Exit Sleep                   */
    delay(120);
    sendCMD(0x29);                                                      /* Display on                   */
    sendCMD(0x2C);                                                      /* write data to Display RAM    */

    fillScreen(BLACK);
}

void TFT::pushData(unsigned char data)
{
    all_pin_low();
    PORTC |= data&0x0f;
    PORTD |= data&0xf0;
}

unsigned char TFT::getData(void)
{
                                                                        /* #if defined(__AVR_ATmega1280 */
                                                                        /* __) || defined(__AVR_ATmega2 */
                                                                        /* 560__)                       */
    unsigned char data=0;
    delay(1);
    data = PINC&0x0f;
    data |= PIND&0xf0;
    return data;
}

void TFT::all_pin_input(void)
{
    DDRC &=~ 0x0f;
    DDRD &=~ 0xf0;
}

void TFT::all_pin_output(void)
{

    DDRC = 0x0f;
    DDRD = 0xf0;

}

void TFT::all_pin_low(void)
{

    PORTC &=~ 0x0f;
    PORTD &=~ 0xf0;

}

void TFT::setCol(unsigned int StartCol,unsigned int EndCol)
{
    sendCMD(0x2A);                                                      /* Column Command address       */
    sendData(StartCol);
    sendData(EndCol);
}

void TFT::setPage(unsigned int StartPage,unsigned int EndPage)
{
    sendCMD(0x2B);                                                      /* Column Command address       */
    sendData(StartPage);
    sendData(EndPage);
}

void TFT::readReg(unsigned char Addr,unsigned char howMany)
{
    sendCMD(Addr);                                                      /* Column Command address       */
    for(unsigned char i=0;i<howMany;i++)
    {
        readData();
    }
}

void TFT::fillScreen(unsigned int color)
{
    for(unsigned char i=0;i<2;i++)
    {
        for(unsigned int f=0;f<38400;f++)
        {
            sendData(color);
        }
    }
}


void TFT::fillScreen(void)
{
    for(unsigned char i=0;i<2;i++)
    {
        for(unsigned int f=0;f<38400;f++)
        {
            sendData(BLACK);
        }
    }
}

void TFT::setXY(unsigned int poX, unsigned int poY)
{
    setCol(poX,poX);
    setPage(poY,poY);
    sendCMD(0x2c);
}

void TFT::setPixel(unsigned int poX, unsigned int poY,unsigned int color)
{
    setXY(poX,poY);
    sendData(color);
}

void TFT::drawLine(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int color)
{
    if(x0>x1 && y0>y1)
    {
        unsigned int temp = x0;
        x0 = x1;
        x1 = temp;
        temp = y0;
        y0 = y1;
        y1 = temp;
    }
    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = dx+dy, e2;                                                /* error value e_xy             */
    for (;;){                                                           /* loop                         */
        setPixel(x0,y0,color);
        e2 = 2*err;
        if (e2 >= dy) {                                                 /* e_xy+e_x > 0                 */
            if (x0 == x1) break;
            err += dy; x0 += sx;
        }
        if (e2 <= dx) {                                                 /* e_xy+e_y < 0                 */
            if (y0 == y1) break;
            err += dx; y0 += sy;
        }
    }
}

void TFT::drawVerticalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color)
{
    setCol(poX,poX);
    setPage(poY,poY+length);
    sendCMD(0x2c);
    for(int i=0; i<length; i++)
    sendData(color);
}

void  TFT::drawHorizontalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color)
{
    setCol(poX,poX + length);
    setPage(poY,poY);
    sendCMD(0x2c);
    for(int i=0; i<length; i++)
    sendData(color);
}

void TFT::drawRectangle(unsigned int poX, unsigned int poY, unsigned int length,unsigned int width,unsigned int color)
{
    drawHorizontalLine(poX, poY, length, color);
    drawHorizontalLine(poX, poY+width, length, color);
    drawVerticalLine(poX, poY, width,color);
    drawVerticalLine(poX + length, poY, width,color);

}

void TFT::fillRectangle(unsigned int poX, unsigned int poY, unsigned int length, unsigned int width, unsigned int color)
{
    for(unsigned int i=0;i<width;i++)
    {
        drawHorizontalLine(poX, poY+i, length, color);
    }

}

void TFT::drawCircle(int poX, int poY, int r,unsigned int color)
{
    int x = -r, y = 0, err = 2-2*r, e2;
    do {
        setPixel(poX-x, poY+y,color);
        setPixel(poX+x, poY+y,color);
        setPixel(poX+x, poY-y,color);
        setPixel(poX-x, poY-y,color);
        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    } while (x <= 0);
}

void TFT::fillCircle(int poX, int poY, int r,unsigned int color)
{
    int x = -r, y = 0, err = 2-2*r, e2;
    do {

        drawVerticalLine(poX-x,poY-y,2*y,color);
        drawVerticalLine(poX+x,poY-y,2*y,color);

        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    } while (x <= 0);

}

void TFT::drawChar(unsigned char ascii,unsigned int poX, unsigned int poY,unsigned int size, unsigned int fgcolor)
{
	if((ascii>=32)&&(ascii<=127))
	{
		ascii-=32;
	}
	else
	{
		ascii = '?'-32;
	}
    for (int i =0; i<5; i++ ) {
        int line = pgm_read_byte(font5x7+(ascii*5)+i);
        for (int j = 0; j<8; j++) {
            if (line & 0x1) {
                if (size == 1)                                          /* default size                 */
                setPixel(poX+i, poY+j, fgcolor);
                else {                                                  /* big size                     */
                    fillRectangle(poX+i*size, poY+j*size, size, size, fgcolor);
                }
            }
            line >>= 1;
        }
    }
}

void TFT::drawString(char *string,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor)
{
    while(*string)
    {
        drawChar(*string, poX, poY, size, fgcolor);
        *string++;

        if(poX < MAX_X)
        {
            poX += FONT_SPACE*size;                                         /* Move cursor right            */
        }
    }
}

void TFT::drawTraingle(int poX1, int poY1, int poX2, int poY2, int poX3, int poY3, unsigned int color)
{
    drawLine(poX1, poY1, poX2, poY2,color);
    drawLine(poX1, poY1, poX3, poY3,color);
    drawLine(poX2, poY2, poX3, poY3,color);
}

unsigned char TFT::drawNumber(long long_num,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor)
{
    unsigned char char_buffer[10] = "";
    unsigned char i = 0;
    unsigned char f = 0;

    if (long_num < 0)
    {
        f=1;
        drawChar('-',poX, poY, size, fgcolor);
        long_num = -long_num;
        if(poX < MAX_X)
        {
            poX += FONT_SPACE*size;                                         /* Move cursor right            */
        }
    }
    else if (long_num == 0)
    {
        f=1;
        drawChar('0',poX, poY, size, fgcolor);
        return f;
        if(poX < MAX_X)
        {
            poX += FONT_SPACE*size;                                         /* Move cursor right            */
        }
    }


    while (long_num > 0)
    {
        char_buffer[i++] = long_num % 10;
        long_num /= 10;
    }

    f = f+i;
    for(; i > 0; i--)
    {
        drawChar('0'+ char_buffer[i - 1],poX, poY, size, fgcolor);
        if(poX < MAX_X)
        {
            poX+=FONT_SPACE*size;                                           /* Move cursor right            */
        }
    }
    return f;
}

unsigned char TFT::drawFloat(float floatNumber,unsigned char decimal,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor)
{
    unsigned int temp=0;
    float decy=0.0;
    float rounding = 0.5;
    unsigned char f=0;
    if(floatNumber<0.0)
    {
        drawChar('-',poX, poY, size, fgcolor);
        floatNumber = -floatNumber;
        if(poX < MAX_X)
        {
            poX+=FONT_SPACE*size;                                           /* Move cursor right            */
        }
        f =1;
    }
    for (unsigned char i=0; i<decimal; ++i)
    {
        rounding /= 10.0;
    }
    floatNumber += rounding;

    temp = (unsigned int)floatNumber;
    unsigned char howlong=drawNumber(temp,poX, poY, size, fgcolor);
    f += howlong;
    if((poX+8*size*howlong) < MAX_X)
    {
        poX+=FONT_SPACE*size*howlong;                                       /* Move cursor right            */
    }

    if(decimal>0)
    {
        drawChar('.',poX, poY, size, fgcolor);
        if(poX < MAX_X)
        {
            poX+=FONT_SPACE*size;                                           /* Move cursor right            */
        }
        f +=1;
    }
    decy = floatNumber-temp;                                            /* decimal part,                */
    for(unsigned char i=0;i<decimal;i++)                                /* 4                            */
    {
        decy *=10;                                                      /* for the next decimal         */
        temp = decy;                                                    /* get the decimal              */
        drawNumber(temp,poX, poY, size, fgcolor);
        floatNumber = -floatNumber;
        if(poX < MAX_X)
        {
            poX+=FONT_SPACE*size;                                           /* Move cursor right            */
        }
        decy -= temp;
    }
    f +=decimal;
    return f;
}

unsigned char TFT::drawFloat(float floatNumber,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor)
{
    unsigned char decimal=2;
    unsigned int temp=0;
    float decy=0.0;
    float rounding = 0.5;
    unsigned char f=0;
    if(floatNumber<0.0)
    {
        drawChar('-',poX, poY, size, fgcolor);
        floatNumber = -floatNumber;
        if(poX < MAX_X)
        {
            poX+=FONT_SPACE*size;                                           /* Move cursor right            */
        }
        f =1;
    }
    for (unsigned char i=0; i<decimal; ++i)
    {
        rounding /= 10.0;
    }
    floatNumber += rounding;

    temp = (unsigned int)floatNumber;
    unsigned char howlong=drawNumber(temp,poX, poY, size, fgcolor);
    f += howlong;
    if((poX+8*size*howlong) < MAX_X)
    {
        poX+=FONT_SPACE*size*howlong;                                       /* Move cursor right            */
    }


    if(decimal>0)
    {
        drawChar('.',poX, poY, size, fgcolor);
        if(poX < MAX_X)
        {
            poX += FONT_SPACE*size;                                         /* Move cursor right            */
        }
        f +=1;
    }
    decy = floatNumber-temp;                                            /* decimal part,                */
    for(unsigned char i=0;i<decimal;i++)                                /* 4                            */
    {
        decy *=10;                                                      /* for the next decimal         */
        temp = decy;                                                    /* get the decimal              */
        drawNumber(temp,poX, poY, size, fgcolor);
        floatNumber = -floatNumber;
        if(poX < MAX_X)
        {
            poX += FONT_SPACE*size;                                         /* Move cursor right            */
        }
        decy -= temp;
    }
    f +=decimal;
    return f;
}

TFT Tft=TFT();

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
