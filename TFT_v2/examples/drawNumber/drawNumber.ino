// draw number - Demonstrate drawNumber and drawFloat
#include <stdint.h>
#include <TFT_v2.h>

void setup()
{
  Tft.init();
  Tft.drawNumber(1024, 0, 0, 1, RED);
  Tft.drawNumber(1024, 10, 20, 2, BLUE);
  Tft.drawNumber(1024, 40, 50, 3, GREEN);
  Tft.drawNumber(1024, 60, 90, 4, BLUE);
  Tft.drawFloat(1.23, 80, 130, 4, YELLOW);
  Tft.drawFloat(1.2345, 4, 80, 180, 4, BLUE);
  Tft.drawNumber(123456789, 10, 250, 3, RED);
}

void loop()
{
  
}
