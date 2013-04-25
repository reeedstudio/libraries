
/* 
 * Example of using the ChainableRGB library for controlling a Grove RGB.
 * This code fades in an out colors in a strip of leds.
 */


#include <ChainableLED.h>

#define NUM_LEDS  5

ChainableLED leds(7, 8, NUM_LEDS);

void setup()
{
}

byte power = 0;

void loop()
{
  for (byte i=0; i<NUM_LEDS; i++)
  {
    if (i%2 == 0)
      leds.setColorRGB(i, power, 0, 0);
    else
      leds.setColorRGB(i, 0, 255-power, 0);
  }  
  power+= 10;
  
  delay(10);
}

