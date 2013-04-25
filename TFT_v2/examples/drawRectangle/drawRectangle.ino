// Draw Boxes - Demonstrate drawRectangle and fillRectangle

#include <stdint.h>
#include <TFT_v2.h>

void setup()
{

Tft.init();  //init TFT library

/*  Demo of 
    drawRectangle(unsigned int poX, unsigned int poY, unsigned int length,unsigned int width,unsigned int color);
    fillRectangle(unsigned int poX, unsigned int poY, unsigned int length, unsigned int width, unsigned int color);
*/
 Tft.drawRectangle(10, 3, 200,60,BLUE);
 Tft.fillRectangle(0, 80, 100,65,YELLOW);
 Tft.drawRectangle(30, 160, 60, 60,RED);
}

void loop()
{
  
}
