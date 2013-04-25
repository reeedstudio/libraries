/*
 Rainbowduino v3.0 Library examples:

 Print Shapes on 2D plane (8x8 matrix)

*/

#include <Rainbowduino.h>

void setup()
{
  Rb.init();
}

unsigned char x,y,z;

void loop()
{

     Rb.drawCircle(3, 4, 3, 0xFF0000); // draw a red circle of radius 3 at (3,4).
     delay(1000);
     Rb.blankDisplay();
  
     Rb.fillCircle(3, 4, 2, 0x0000FF); // draw a filled blue circle of radius 2 at (3,4).
     delay(1000);
     Rb.blankDisplay();

     Rb.drawLine(0, 0, 7, 7, 0x00FF00); // draw a line from (0,0) to (7,7).
     delay(1000);
     Rb.blankDisplay();

     Rb.drawVerticalLine(0, 0, 7, random(0xFFFFFF)); // draw a vertical line from (0,0) of length 7 pixels
     delay(1000);
     Rb.blankDisplay();

     Rb.drawHorizontalLine(0, 0, 7, random(0xFFFFFF)); // draw a horizontal line from (0,0) of length 7 pixels
     delay(1000);
     Rb.blankDisplay();

     Rb.drawRectangle(0, 0, 4, 6, random(0xFFFFFF)); // draw a rectangle line from (0,0) of length 4 and width 6 pixels
     delay(1000);
     Rb.blankDisplay();

     Rb.fillRectangle(0, 0, 7, 7, random(0xFFFFFF)); // draw a filled rectangle line from (0,0) of length and width 7 pixels
     delay(1000);
     Rb.blankDisplay();
  
}




