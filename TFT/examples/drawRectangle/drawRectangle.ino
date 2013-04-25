/*  Draw Boxes - Demonstrate drawRectangle and fillRectangle
    fillScreen(INT16U XL,INT16U XR,INT16U YU,INT16U YD,INT16U color);
    fillRectangle(INT16U poX, INT16U poY, INT16U length, INT16U width, INT16U color);
    drawRectangle(INT16U poX, INT16U poY, INT16U length,INT16U width,INT16U color);
*/
#include <stdint.h>
#include <TFT.h>

void setup()
{

    Tft.init();  // init TFT library
  //  Tft.fillScreen(80, 160, 50, 150,RED);
  /*  Tft.fillScreen();
    Tft.fillRectangle(30, 120, 100,65,YELLOW);
    Tft.drawRectangle(100, 170, 120,60,BLUE);*/
    Tft.fillScreen();
    Tft.fillScreen(12, 112, 110, 210, GREEN);
//    Tft.drawString("CALL", 12+13, 110+40, 3, BLACK);
    Tft.fillScreen(128,228,110,210,YELLOW);
  //  Tft.drawString("SMS", 128+20, 110+40, 3, BLACK);
}

void loop()
{
  
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/