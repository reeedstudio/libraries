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

#include "Rainbowduino.h"

//Frame Buffer -- placed in RAM
unsigned char frameBuffer[3][8][8]=
{

    {//blue
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0}
    },

    {//green
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0}
    },

        {//red
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0}
    }
};

//Matrix to Cube mapping 
unsigned char ZX[4][4]= 
{
 {7,7,0,0},
 {6,6,1,1},
 {5,5,2,2},
 {4,4,3,3},
};

unsigned char YX[4][4] = 
{
 {0,7,0,7},
 {1,6,1,6},
 {2,5,2,5},
 {3,4,3,4}
};


unsigned char lineDrive=0; //used within ISR

//Init the Port lines and invoke timer 1 configuration

void Rainbowduino::init()
{
    DDR_Lines |= BIT_Lines;
    PORT_Lines &=~ BIT_Lines;

    DDRD |= 0x04;

    DDR_Data |= BIT_Data;
    DDR_Clk |= BIT_Clk;
    PORT_Data &=~ BIT_Data;
    PORT_Clk &=~ BIT_Clk;

    DDRB |= 0x20;

    clearDisplay();
    init_timer1();  //Configure interrupt
}

//Configure Timer 1 ISR for periodic 100uS interrupts

void Rainbowduino::init_timer1(void)		//initialize Timer1 to overflow every  100uS
{
    TCCR1A = 0;                 		// clear control register A
    TCCR1B = _BV(WGM13);        		// set mode as phase and frequency correct pwm, stop the timer
    ICR1 = 10000;               		//(XTAL * microseconds) / 2000000  1mS cycles
    TIMSK1 = _BV(TOIE1);
    TCNT1 = 0;
    TCCR1B |= _BV(CS10);
    sei();                      		//enable global interrupt
}

//Routine to send 16bit data to MY9221 driver chips

void Rainbowduino::send16bitData(unsigned int data)
{
    for(unsigned char i=0;i<16;i++)
    {
        if(data&0x8000)
        {
            PORT_Data |= BIT_Data;
        }
        else
        {
            PORT_Data &=~ BIT_Data;
        }

        PORT_Clk ^= BIT_Clk;
        data <<= 1;
    }
}

//latch routine for MY9221 data exchange

void Rainbowduino::latchData(void)
{
    PORT_Data &=~ BIT_Data;

    delayMicroseconds(10);
    switchOffDrive;
    for(unsigned char i=0;i<8;i++)
    {
        PORT_Data ^= BIT_Data;
    }
} 

void Rainbowduino::switchOnDrive(unsigned char line)
{
  unsigned char LineBits = ((line)<<4);
  PORT_Lines &=~ BIT_Lines;
  PORT_Lines |= LineBits;
  PORT_Lines |= 0x80;
}

//clear MY9221 lines. Internally used for avoiding flicker. This is not the same as blank disply.

void Rainbowduino::clearDisplay(void)
{
    unsigned char i=0;
    unsigned char f=0;

    send16bitData(CmdMode);
    PORT_Data &=~ BIT_Data;
    for(i=0;i<192;i++)
    {
        {
            PORT_Clk ^= BIT_Clk;
        }
    }

    send16bitData(CmdMode);
    PORT_Data &=~ BIT_Data;
    for(i=0;i<192;i++)
    {
        {
            PORT_Clk ^= BIT_Clk;
        }
    }
    latchData();

}

//blank all pixels

void Rainbowduino::blankDisplay(void)
{
   for(unsigned char x=0;x<=7;x++)
     {
      for(unsigned char y=0;y<=7;y++)
       {
        frameBuffer[0][x][y] = 0x00;
        frameBuffer[1][x][y] = 0x00;
        frameBuffer[2][x][y] = 0x00;
       }
     }
}   

//set the pixel (X,Y) of RGB matrix with colour 24-bit RGB Colour 
void Rainbowduino::setPixelXY(unsigned char x, unsigned char y, uint32_t colorRGB /*24-bit RGB Color*/)
{
    if(x > 7 || y > 7)
    {
     // Do nothing.
     // This check is used to avoid writing to out-of-bound pixels by graphics function. 
     // But this might slow down setting pixels (remove this check if fast disply is desired)
    }
    else
    {
    colorRGB = (colorRGB & 0x00FFFFFF);
    frameBuffer[0][x][y]=(colorRGB & 0x0000FF); //channel Blue
    colorRGB = (colorRGB >> 8);
    frameBuffer[1][x][y]=(colorRGB & 0x0000FF); //channel Green
    colorRGB = (colorRGB >> 8);
    frameBuffer[2][x][y]=(colorRGB & 0x0000FF); //channel Red
    }
}

//set the pixel (X,Y) of RGB matrix with colours R,G,B 
void Rainbowduino::setPixelXY(unsigned char x, unsigned char y, unsigned char  colorR,  unsigned char colorG, unsigned char colorB)
{
    frameBuffer[0][x][y]= colorB ; //channel Blue
    frameBuffer[1][x][y]= colorG ; //channel Green
    frameBuffer[2][x][y]= colorR ; //channel Red
}

//set the pixel (Z,X,Y) of RGB Cube with colour 24-bit RGB Colour 
void Rainbowduino::setPixelZXY(unsigned char z, unsigned char x, unsigned char y, uint32_t colorRGB /*24-bit RGB Color*/)
{
    setPixelXY(ZX[z][x], YX[y][x], colorRGB);
}

//set the pixel (Z,X,Y) of RGB Cube with colours R,G,B 
void Rainbowduino::setPixelZXY(unsigned char z, unsigned char x, unsigned char y, unsigned char colorR,  unsigned char colorG, unsigned char colorB)
{
    setPixelXY(ZX[z][x], YX[y][x], colorR, colorG, colorB);
}

//fill the frame buffer starting from 'start' to ending at 'end' with values in colorRGB array.
//Pixels range is from 0,1,2,.....61,62
void Rainbowduino::setPixelXY(unsigned char start, unsigned char end, uint32_t *colorRGB)
{
  unsigned char ci =0;
  for(unsigned char i=start; i<=end; i++)
    {
      setPixelXY(i/8, i % 8, colorRGB[ci]);
      ci++; 
    }

}

void Rainbowduino::drawCircle(int poX, int poY, int r, uint32_t color)
{
    int x = -r, y = 0, err = 2-2*r, e2;
    do {
        setPixelXY(poX-x, poY+y,color);
        setPixelXY(poX+x, poY+y,color);
        setPixelXY(poX+x, poY-y,color);
        setPixelXY(poX-x, poY-y,color);
        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    }
    while (x <= 0);
}

void Rainbowduino::fillCircle(int poX, int poY, int r, uint32_t color)
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
    }
    while (x <= 0);

}

void Rainbowduino::drawLine(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1, uint32_t color)
{
    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = dx+dy, e2; /* error value e_xy */
    for (;;){ /* loop */
        setPixelXY(x0,y0,color);
        e2 = 2*err;
        if (e2 >= dy) { /* e_xy+e_x > 0 */
            if (x0 == x1) break;
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) { /* e_xy+e_y < 0 */
            if (y0 == y1) break;
            err += dx;
            y0 += sy;
        }
    }
}

void Rainbowduino::drawVerticalLine(unsigned int poX, unsigned int poY,unsigned int length, uint32_t color)
{
   drawLine(poX,poY,poX,poY+length-1,color);
}

void Rainbowduino::drawHorizontalLine(unsigned int poX, unsigned int poY,unsigned int length, uint32_t color)
{
   drawLine(poX,poY,poX+length-1,poY,color);
}

void Rainbowduino::drawRectangle(unsigned int poX, unsigned int poY, unsigned int length,unsigned int width, uint32_t color)
{
    drawHorizontalLine(poX, poY, length, color);
    drawHorizontalLine(poX, poY+width-1, length, color);
    drawVerticalLine(poX, poY, width,color);
    drawVerticalLine(poX+length-1, poY, width,color);
}

void Rainbowduino::fillRectangle(unsigned int poX, unsigned int poY, unsigned int length, unsigned int width, uint32_t color)
{
    for(unsigned int i=0;i<width;i++)
    {
        drawHorizontalLine(poX, poY+i, length, color);
    }
}

void Rainbowduino::drawChar(unsigned char ascii,unsigned int poX, unsigned int poY,  unsigned int colorRGB)
{
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
                setPixelXY(poY+f, poX+i, colorRGB);
            }

        }
    }
}


//Timer1 Interrupt Service Routine
//All frameBuffer data exchange happens here

ISR(TIMER1_OVF_vect)          
{
    //An Ideal ISR has to be short and not make any function calls
    //But, in this case only data exchange happens.

    Rb.clearDisplay(); //clear current display to avoid the next line dim light

    unsigned char line = (lineDrive & 0x07);

    Rb.send16bitData(CmdMode);

    Rb.send16bitData(frameBuffer[0][line][7]);
    Rb.send16bitData(frameBuffer[0][line][6]);
    Rb.send16bitData(frameBuffer[0][line][5]);
    Rb.send16bitData(frameBuffer[0][line][4]);
    Rb.send16bitData(frameBuffer[0][line][3]);
    Rb.send16bitData(frameBuffer[0][line][2]);
    Rb.send16bitData(frameBuffer[0][line][1]);
    Rb.send16bitData(frameBuffer[0][line][0]);

    Rb.send16bitData(frameBuffer[1][line][7]);
    Rb.send16bitData(frameBuffer[1][line][6]);
    Rb.send16bitData(frameBuffer[1][line][5]);
    Rb.send16bitData(frameBuffer[1][line][4]);

    Rb.send16bitData(CmdMode);

    Rb.send16bitData(frameBuffer[1][line][3]);
    Rb.send16bitData(frameBuffer[1][line][2]);
    Rb.send16bitData(frameBuffer[1][line][1]);
    Rb.send16bitData(frameBuffer[1][line][0]);

    Rb.send16bitData(frameBuffer[2][line][7]);
    Rb.send16bitData(frameBuffer[2][line][6]);
    Rb.send16bitData(frameBuffer[2][line][5]);
    Rb.send16bitData(frameBuffer[2][line][4]);
    Rb.send16bitData(frameBuffer[2][line][3]);
    Rb.send16bitData(frameBuffer[2][line][2]);
    Rb.send16bitData(frameBuffer[2][line][1]);
    Rb.send16bitData(frameBuffer[2][line][0]);

    Rb.latchData();
    Rb.switchOnDrive(line);
    lineDrive++;

    PORTD &=~ 0x04;
}

Rainbowduino Rb;







