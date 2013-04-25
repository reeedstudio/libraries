
/* 
 * Example of using the ChainableRGB library for controlling a Grove RGB.
 * This code puts a red dot (led) moving along a strip of blue dots.
 */

#include <ChainableLED.h>

#define NUM_LEDS  5

ChainableLED leds(7, 8, NUM_LEDS);

void setup()
{
}

byte pos = 0;

void loop()
{
  for (byte i=0; i<NUM_LEDS; i++)
  {
    if (i==pos)
      leds.setColorRGB(i, 255, 0, 0);  
    else
      leds.setColorRGB(i, 0, 0, 255); 
  }
  delay(250);
  
  pos = (pos+1) % NUM_LEDS;
}

