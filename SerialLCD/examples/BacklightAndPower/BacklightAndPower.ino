/*
  SerialLCD Library - Cursor
 
 Demonstrates the use a 16x2 LCD SerialLCD driver from Seeedstudio.
 
 This sketch prints "Hello World!" to the LCD and
 uses the cursor()  and noCursor() methods to turn
 on and off the cursor.
 
 Library originally added 16 Dec. 2010
 by Jimbo.we 
 Library modified 15 March,2012
 by Frankie.Chu
 http://www.seeedstudio.com
 */

// include the library code:
#include "SerialLCD.h"
#include <SoftwareSerial.h> //this is a must

// initialize the library
SerialLCD slcd(11,12);//this is a must, assign soft serial pins

void setup() {
  // set up
  slcd.begin();
  // Print a message to the LCD.
  slcd.print("hello, world!");
}

void loop() {
  // Turn off the cursor:
  slcd.noCursor();
  delay(1000);
  slcd.noBacklight();
   // Turn on the cursor:
  slcd.cursor();
  delay(1000);
  slcd.backlight();
  delay(1000);
  slcd.noPower();//power  off
  delay(500);
  slcd.begin();//after power off,LCD should be initialized again.
  slcd.print("hello, world!");
}
