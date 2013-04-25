// Draw Boxes - Demonstrate drawRectangle and fillRectangle

#include <stdint.h>
#include <TFT.h>


void setup()
{

    Tft.init();                                      // init TFT library
}

void loop()
{
    for(int r=0;r<115;r=r+2)                            //set r : 0--115
    {
        Tft.drawCircle(119,160,r,random(0xFFFF));       //draw circle, center:(119, 160), color: random
    }
    delay(10);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/