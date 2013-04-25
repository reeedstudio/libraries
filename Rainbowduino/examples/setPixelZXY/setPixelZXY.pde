/*
 Rainbowduino v3.0 Library examples:

 Sets pixels on 3D plane (4x4x4 cube)

*/

#include <Rainbowduino.h>

void setup()
{
  Rb.init();
}

unsigned char x,y,z;

void loop()
{
  for(z=0;z<4;z++)
  {
   for(y=0;y<4;y++)
   {
    for(x=0;x<4;x++)
    {
      Rb.setPixelZXY(z,x,y,random(0xFF),random(0xFF),random(0xFF)); //uses R, G and B color bytes
      delay(250); //delay to illustrate the postion change. can be removed
    }
   }
  } 
  
Rb.blankDisplay(); //Clear the LEDs (make all blank)

for(z=0;z<4;z++)
  {
   for(y=0;y<4;y++)
   {
    for(x=0;x<4;x++)
    {
      Rb.setPixelZXY(z,x,y,random(0xFFFFFF)); //uses 24-bit RGB color
      delay(250); //delay to illustrate the postion change. can be removed
    }
   }
  } 

Rb.blankDisplay(); //Clear the LEDs (make all blank)
for(y=0;y<4;y++)
   {
    for(x=0;x<4;x++)
    {
      Rb.setPixelZXY(0,x,y,0XFF0000); //Paint layer 0 (z-0) Red completely 
    }
   }
for(y=0;y<4;y++)
   {
    for(x=0;x<4;x++)
    {
      Rb.setPixelZXY(1,x,y,0X00FF00); //Paint layer 1 (z-1) Green completely 
    }
   }
for(y=0;y<4;y++)
   {
    for(x=0;x<4;x++)
    {
      Rb.setPixelZXY(2,x,y,0X0000FF); //Paint layer 2 (z-2) Blue completely 
    }
   }

for(y=0;y<4;y++)
   {
    for(x=0;x<4;x++)
    {
      Rb.setPixelZXY(3,x,y,0XFFFFF); //Paint layer 3 (z-3) White completely 
    }
   }
   
delay(5000);
Rb.blankDisplay(); //Clear the LEDs (make all blank)
}

