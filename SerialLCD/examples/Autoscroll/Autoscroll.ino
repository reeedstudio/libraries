/*
  SerialLCD Library - Autoscroll
 
 Demonstrates the use a 16x2 LCD SerialLCD driver from Seeedstudio.

 This sketch demonstrates the use of the autoscroll()
 and noAutoscroll() functions to make new text scroll or not.
 
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
  slcd.begin();
}

void loop() {
  // set the cursor to (0,0):
  slcd.setCursor(0, 0);
  // print from 0 to 9:
  for (int thisChar = 0; thisChar < 10; thisChar++) {
   slcd.print(thisChar,DEC);
   delay(500);
  }

  // set the cursor to (16,1):
  slcd.setCursor(16,1);
  // set the display to automatically scroll:
  slcd.autoscroll();
  // print from 0 to 9:
  for (int thisChar = 0; thisChar < 10; thisChar++) {
    slcd.print(thisChar,DEC);
    delay(500);
  }
  // turn off automatic scrolling
  slcd.noAutoscroll();
  
  // clear screen for the next loop:
  slcd.clear();
}
