/*
  SerialLCD Library - Blink

  Demonstrates the use a 16x2 LCD SerialLCD driver from Seeedstudio.

 This sketch prints "Hello World!" to the LCD and makes the 
 cursor block blink.
 
 Library originally added 16 Dec. 2010
 by Jimbo.we 
 http://www.seeedstudio.com
 */

// include the library code:
#include <SerialLCD.h>
#include <SoftwareSerial.h> //this is a must

// initialize the library
SerialLCD slcd(11,12);//this is a must, assign soft serial pins

void setup() {
  // set up : 
  slcd.begin();
  // Print a message to the LCD.
  slcd.print("hello, world!");
}

void loop() {
  // Turn off the blinking cursor:
  slcd.noBlink();
  delay(1000);
   // Turn on the blinking cursor:
  slcd.blink();
  delay(1000);
}
