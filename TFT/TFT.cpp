/*
  ST7781R TFT Library. 

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

#include "TFT.h" 

#define FONT_SPACE 6
#define FONT_X 8
#define FONT_Y 8


void TFT::pushData(unsigned char data)
{
    all_pin_low();
#ifdef SEEEDUINO
    PORTD |= (data<<2);
    PORTB |= (data>>6);
#endif

#ifdef MEGA

    PORTE |= ((data<<4) & (0x30));
    PORTG |= ((data<<3) & (0x20));
    PORTE |= ((data & 0x08));
    PORTH |= ((data>>1) & (0x78));

#endif

#ifdef MAPLE
#endif
}

unsigned char TFT::getData(void)
{
    unsigned char data=0;
    delay(1);
    data |= ((PIND&0xfc)>>2);
    data |= ((PINB&0x03)<<6);
    return data;
}

void TFT::sendCommand(unsigned int index)
{
    CS_LOW;
    RS_LOW;
    RD_HIGH;
    WR_HIGH;

    WR_LOW;
    pushData(0);
    WR_HIGH;
    WR_LOW;
    pushData(index&0xff);
    WR_HIGH;

    CS_HIGH;
}

void TFT::sendData(unsigned int data)
{
    CS_LOW;
    RS_HIGH;
    RD_HIGH;

    WR_LOW;
    pushData((data&0xff00)>>8);
    WR_HIGH;

    WR_LOW;
    pushData(data&0xff);
    WR_HIGH;

    CS_HIGH;
}

unsigned int TFT::readRegister(unsigned int index)
{
    unsigned int data=0;

    CS_LOW;
    RS_LOW;
    RD_HIGH;

    all_pin_output();

    WR_LOW;
    pushData(0);
    WR_HIGH;

    WR_LOW;
    pushData(index);
    WR_HIGH;

    all_pin_input();
    all_pin_low();
    RS_HIGH;

    RD_LOW;
    RD_HIGH;
    data |= getData()<<8;

    RD_LOW;
    RD_HIGH;
    data |= getData();

    CS_HIGH;
    all_pin_output();
    return data;
}

void  TFT::init (void)
{
    CS_OUTPUT;
    RD_OUTPUT;
    WR_OUTPUT;
    RS_OUTPUT;

    Tft.all_pin_output();
    Tft.all_pin_low();

    delay(100);


    sendCommand(0x0001);
    sendData(0x0100);
    sendCommand(0x0002);
    sendData(0x0700);
    sendCommand(0x0003);
    sendData(0x1030);
    sendCommand(0x0004);
    sendData(0x0000);
    sendCommand(0x0008);
    sendData(0x0302);
    sendCommand(0x000A);
    sendData(0x0000);
    sendCommand(0x000C);
    sendData(0x0000);
    sendCommand(0x000D);
    sendData(0x0000);
    sendCommand(0x000F);
    sendData(0x0000);

    delay(100);

    sendCommand(0x0030);
    sendData(0x0000);
    sendCommand(0x0031);
    sendData(0x0405);
    sendCommand(0x0032);
    sendData(0x0203);
    sendCommand(0x0035);
    sendData(0x0004);
    sendCommand(0x0036);
    sendData(0x0B07);
    sendCommand(0x0037);
    sendData(0x0000);
    sendCommand(0x0038);
    sendData(0x0405);
    sendCommand(0x0039);
    sendData(0x0203);
    sendCommand(0x003c);
    sendData(0x0004);
    sendCommand(0x003d);
    sendData(0x0B07);
    sendCommand(0x0020);
    sendData(0x0000);
    sendCommand(0x0021);
    sendData(0x0000);
    sendCommand(0x0050);
    sendData(0x0000);
    sendCommand(0x0051);
    sendData(0x00ef);
    sendCommand(0x0052);
    sendData(0x0000);
    sendCommand(0x0053);
    sendData(0x013f);

    delay(100);

    sendCommand(0x0060);
    sendData(0xa700);
    sendCommand(0x0061);
    sendData(0x0001);
    sendCommand(0x0090);
    sendData(0x003A);
    sendCommand(0x0095);
    sendData(0x021E);
    sendCommand(0x0080);
    sendData(0x0000);
    sendCommand(0x0081);
    sendData(0x0000);
    sendCommand(0x0082);
    sendData(0x0000);
    sendCommand(0x0083);
    sendData(0x0000);
    sendCommand(0x0084);
    sendData(0x0000);
    sendCommand(0x0085);
    sendData(0x0000);
    sendCommand(0x00FF);
    sendData(0x0001);
    sendCommand(0x00B0);
    sendData(0x140D);
    sendCommand(0x00FF);
    sendData(0x0000);
    delay(100);
    sendCommand(0x0007);
    sendData(0x0133);
    delay(50);
    exitStandBy();
    sendCommand(0x0022);

    //paint screen black
    for(unsigned char i=0;i<2;i++)
    {
        for(unsigned int f=0;f<38400;f++)
        {
            sendData(BLACK);
        }
    }
}

void TFT::exitStandBy(void)
{
    sendCommand(0x0010);
    sendData(0x14E0);
    delay(100);
    sendCommand(0x0007);
    sendData(0x0133);
}

void TFT::setOrientation(unsigned int HV)//horizontal or vertical
{
    sendCommand(0x03);
    if(HV==1)//vertical
    {
        sendData(0x5038);
    }
    else//horizontal
    {
        sendData(0x5030);
    }
    sendCommand(0x0022); //Start to write to display RAM
}

void TFT::setXY(unsigned int poX, unsigned int poY)
{
    sendCommand(0x0020);//X
    sendData(poX);
    sendCommand(0x0021);//Y
    sendData(poY);
    sendCommand(0x0022);//Start to write to display RAM
}

void TFT::setPixel(unsigned int poX, unsigned int poY,unsigned int color)
{
    setXY(poX,poY);
    sendData(color);
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


void TFT::drawLine(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int color)
{
    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = dx+dy, e2; /* error value e_xy */
    for (;;){ /* loop */
        setPixel(x0,y0,color);
        e2 = 2*err;
        if (e2 >= dy) { /* e_xy+e_x > 0 */
            if (x0 == x1) break;
            err += dy; x0 += sx;
        }
        if (e2 <= dx) { /* e_xy+e_y < 0 */
            if (y0 == y1) break;
            err += dx; y0 += sy;
        }
    }
}


void TFT::drawVerticalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color)
{
    setXY(poX,poY);
    setOrientation(1);
    if(length+poY>MAX_Y)
    {
        length=MAX_Y-poY;
    }

    for(unsigned int i=0;i<length;i++)
    {
        sendData(color);
    }
}

void  TFT::drawHorizontalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color)
{
    setXY(poX,poY);
    setOrientation(0);
    if(length+poX>MAX_X)
    {
        length=MAX_X-poX;
    }
    for(unsigned int i=0;i<length;i++)
    {
        sendData(color);
    }
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

void TFT::drawChar(unsigned char ascii,unsigned int poX, unsigned int poY,unsigned int size, unsigned int fgcolor)
{
    setXY(poX,poY);
    setOrientation(1);
    if((ascii < 0x20)||(ascii > 0x7e))//Unsupported char.
    {
        ascii = '?';
    }
    for(unsigned char i=0;i<8;i++)
    {
        unsigned char temp = pgm_read_byte(&simpleFont[ascii-0x20][i]);
        for(unsigned char f=0;f<8;f++)
        {
            if((temp>>f)&0x01)
            {
                fillRectangle(poX+i*size, poY+f*size, size, size, fgcolor);
            }

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
            poX+=FONT_SPACE*size; // Move cursor right
        }

    }
}

void TFT::all_pin_input(void)
{
#ifdef SEEEDUINO
    DDRD &=~ 0xfc;
    DDRB &=~ 0x03;
#endif

#ifdef MEGA
    DDRE &=~ 0x38;
    DDRG &=~ 0x20;
    DDRH &=~ 0x78;
#endif

#ifdef MAPLE

#endif

}

void TFT::all_pin_output(void)
{
#ifdef SEEEDUINO
    DDRD |= 0xfc;
    DDRB |= 0x03;
#endif

#ifdef MEGA
    DDRE |= 0x38;
    DDRG |= 0x20;
    DDRH |= 0x78;
#endif

#ifdef MAPLE

#endif
}

void TFT::all_pin_low(void)
{
#ifdef SEEEDUINO
    PORTD &=~ 0xfc;
    PORTB &=~ 0x03;
#endif

#ifdef MEGA
    PORTE &=~ 0x38;
    PORTG &=~ 0x20;
    PORTH &=~ 0x78;
#endif

#ifdef MAPLE

#endif
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
            poX += FONT_SPACE*size;                                     /* Move cursor right            */
        }
    }
    else if (long_num == 0)
    {
        f=1;
        drawChar('0',poX, poY, size, fgcolor);
        return f;
        if(poX < MAX_X)
        {
            poX += FONT_SPACE*size;                                     /* Move cursor right            */
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
            poX+=FONT_SPACE*size;                                       /* Move cursor right            */
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
            poX+=FONT_SPACE*size;                                       /* Move cursor right            */
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
        poX+=FONT_SPACE*size*howlong;                                   /* Move cursor right            */
    }

    if(decimal>0)
    {
        drawChar('.',poX, poY, size, fgcolor);
        if(poX < MAX_X)
        {
            poX+=FONT_SPACE*size;                                       /* Move cursor right            */
        }
        f +=1;
    }
    decy = floatNumber-temp;                                            /* decimal part,  4             */
    for(unsigned char i=0;i<decimal;i++)                                      
    {
        decy *=10;                                                      /* for the next decimal         */
        temp = decy;                                                    /* get the decimal              */
        drawNumber(temp,poX, poY, size, fgcolor);
        floatNumber = -floatNumber;
        if(poX < MAX_X)
        {
            poX+=FONT_SPACE*size;                                       /* Move cursor right            */
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
    if(floatNumber<0.0)                                                 /* floatNumber < 0              */
    {
        drawChar('-',poX, poY, size, fgcolor);                          /* add a '-'                    */
        floatNumber = -floatNumber;
        if(poX < MAX_X)
        {
            poX+=FONT_SPACE*size;                                       /* Move cursor right            */
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
        poX+=FONT_SPACE*size*howlong;                                   /* Move cursor right            */
    }


    if(decimal>0)
    {
        drawChar('.',poX, poY, size, fgcolor);
        if(poX < MAX_X)
        {
            poX += FONT_SPACE*size;                                     /* Move cursor right            */
        }
        f +=1;
    }
    decy = floatNumber-temp;                                            /* decimal part,                */
    for(unsigned char i=0;i<decimal;i++)
    {
        decy *=10;                                                      /* for the next decimal         */
        temp = decy;                                                    /* get the decimal              */
        drawNumber(temp,poX, poY, size, fgcolor);
        floatNumber = -floatNumber;
        if(poX < MAX_X)
        {
            poX += FONT_SPACE*size;                                     /* Move cursor right            */
        }
        decy -= temp;
    }
    f += decimal;
    return f;
}

void TFT::fillScreen(unsigned int XL, unsigned int XR, unsigned int YU, unsigned int YD, unsigned int color)
{
    fillRectangle(XL, YU, XR-XL, YD-YU, color);
}

void TFT::fillScreen(void)
{
    fillScreen(0, 239, 0, 319, BLACK);
}






TFT Tft=TFT();


