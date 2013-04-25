#include <Wire.h>
#include <SeeedGrayOLED.h>
#include <avr/pgmspace.h>


void setup()
{
  Wire.begin();
  Serial.begin(9600);
  SeeedGrayOled.init();                 // initialize SEEED OLED display
  SeeedGrayOled.setInverseDisplay();    // Set display to inverse mode
  SeeedGrayOled.clearDisplay();         // Clear Display.
  SeeedGrayOled.setVerticalMode();      // Set to vertical mode for displaying text

  for(char i=0; i < 12 ; i++)
  {
  SeeedGrayOled.setTextXY(i,0);  //set Cursor to ith line, 0th column
  SeeedGrayOled.setGrayLevel(i); //Set Grayscale level. Any number between 0 - 15.
  SeeedGrayOled.putString("Hello World"); //Print Hello World
  }
}

void loop()
{
  
}


