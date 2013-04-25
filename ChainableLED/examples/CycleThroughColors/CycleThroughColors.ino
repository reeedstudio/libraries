
/* 
 * Example of using the ChainableRGB library for controlling a Grove RGB.
 * This code cycles through all the colors in an uniform way. This is accomplished using a HSB color space. 
 * comments add By: http://www.seeedstudio.com/
 * Suiable for Grove - Chainable LED 
 */


#include <ChainableLED.h>
//Defines the num of LEDs used, The undefined 
//will be lost control.
#define NUM_LEDS  5

ChainableLED leds(7, 8, NUM_LEDS);//defines the pin used on arduino.

void setup()
{
  //nothing
}

float hue = 0.0;
boolean up = true;

void loop()
{
  for (byte i=0; i<NUM_LEDS; i++)
    leds.setColorHSB(i, hue, 1.0, 0.5);
    
  delay(50);
    
  if (up)
    hue+= 0.025;
  else
    hue-= 0.025;
    
  if (hue>=1.0 && up)
    up = false;
  else if (hue<=0.0 && !up)
    up = true;
}

