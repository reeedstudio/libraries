// Draw Boxes - Demonstrate drawRectangle and fillRectangle

#include <stdint.h>
#include <TouchScreen.h>
#include <TFT_v2.h>

#ifdef SEEEDUINO
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 14   // can be a digital pin, this is A0
  #define XP 17   // can be a digital pin, this is A3 
#endif

#ifdef MEGA
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 54   // can be a digital pin, this is A0
  #define XP 57   // can be a digital pin, this is A3 
#endif 

void setup()
{

Tft.init();  //init TFT library

}

void loop()
{
   for(int r=0;r<115;r=r+2)
   {
       Tft.drawCircle(119,160,r,random(0xFFFF));
   }
  
}
