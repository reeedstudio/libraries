// Draw Circles - Demonstrate drawCircle and fillCircle APIs

#include <stdint.h>
#include <TFT_v2.h>

void setup()
{

Tft.init();  //init TFT library

Tft.drawCircle(100, 100, 30,YELLOW);
Tft.drawCircle(100, 200, 40,CYAN);
Tft.fillCircle(200, 100, 30,RED);
Tft.fillCircle(200, 200, 30,BLUE);
}

void loop()
{
  
}
