/*
 Rainbowduino v3.0 Library examples:  Cube3

 Sets pixels on 3D plane (4x4x4 cube)
*/

#include <Rainbowduino.h>

void setup()
{
  Rb.init(); //initialize Rainbowduino driver
}

unsigned int z,x,y;

void loop()
{
 for(z=0;z<4;z++)
 { 
  for(x=0;x<4;x++)
  {
    for(y=0;y<4;y++)
    {
     //Paint random colors
     Rb.setPixelZXY(z,x,y,random(0xFF),random(0xFF),random(0xFF)); //uses R, G and B color bytes
    }
  }
 }
delay(5000);
Rb.blankDisplay(); //Clear the LEDs (make all blank)
}

