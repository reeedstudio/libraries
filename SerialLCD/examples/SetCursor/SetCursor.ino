/*
  SerialLCD Library - setCursor
 
 Demonstrates the use a 16x2 LCD SerialLCD driver from Seeedstudio.
 
 This sketch prints to all the positions of the LCD using the
 setCursor(col,row) method:
 
 Library originally added 16 Dec. 2010
 by Jimbo.we 
 http://www.seeedstudio.com
 */

// include the library code:
#include <SerialLCD.h>
#include <SoftwareSerial.h> //this is a must

// initialize the library
SerialLCD slcd(11,12);//this is a must, assign soft serial pins

const int numRows = 2;
const int numCols = 16;


void setup() {
  // set up: 
  slcd.begin();
}

void loop() {
  // loop from ASCII 'a' to ASCII 'z':
  for (int thisLetter = 'a'; thisLetter <= 'z'; thisLetter++) {
    // loop over the columns:
    for (int thisCol = 0; thisCol < numRows; thisCol++) {
      // loop over the rows:
      for (int thisRow = 0; thisRow < numCols; thisRow++) {
        // set the cursor position:
        slcd.setCursor(thisRow,thisCol);
        // print the letter:
        slcd.print(thisLetter, BYTE);
        delay(200);
      }
    }
  }
}
