/*
 Rainbowduino v3.0 Library examples:

 Sets pixels on 2D plane (8x8 matrix)

*/

#include <Rainbowduino.h>

uint32_t colorRGB[13] = {0xFFFFFF,0x000000,0xFFFFFF,0x000,0xFF0000,0x00FF00,0x0000FF,0xFF0000,0x00FF00,0x0000FF,0xFF0000,0x00FF00,0x0000FF };

void setup()
{
  Rb.init();
}

unsigned char x,y,z;

void loop()
{
    for(x=0;x<8;x++)
    {
     for(y=0;y<8;y++)
      {
      Rb.setPixelXY(x,y,random(0xFF),random(0xFF),random(0xFF)); //uses R, G and B bytes
      }
    }

    delay(2000);
    Rb.blankDisplay();
    for(x=0;x<8;x++)
    {
     for(y=0;y<8;y++)
      {
      Rb.setPixelXY(x,y,0xFF0000); //use 24-bit color value
      }
    }

    delay(2000);
    Rb.blankDisplay();
    Rb.setPixelXY(2,15,colorRGB);  //Displays the pixels from the array
    delay(2000);
}




