  /*
  SerialLCD Library - TextDirection
 
 Demonstrates the use a 16x2 LCD SerialLCD driver from Seeedstudio.
 
 This sketch demonstrates how to use leftToRight() and rightToLeft()
 to move the cursor.
 
 
 Library originally added 16 Dec. 2010
 by Jimbo.we 
 http://www.seeedstudio.com
 */

// include the library code:
#include <SerialLCD.h>
#include <SoftwareSerial.h> //this is a must

// initialize the library
SerialLCD slcd(11,12);//this is a must, assign soft serial pins

int thisChar = 'a';

void setup() {
  // set up: 
  slcd.begin();
  // turn on the cursor:
  slcd.cursor();
}

void loop() {
  // reverse directions at 'm':
  if (thisChar == 'm') {
    // go right for the next letter
    slcd.rightToLeft(); 
  }
  // reverse again at 's':
  if (thisChar == 's') {
    // go left for the next letter
    slcd.leftToRight(); 
  }
  // reset at 'z':
  if (thisChar > 'z') {
    // go to (0,0):
    slcd.home(); 
    // start again at 0
    thisChar = 'a';
  }
  // print the character
  slcd.print(thisChar, BYTE);
  // wait a second:
  delay(1000);
  // increment the letter:
  thisChar++;
}






