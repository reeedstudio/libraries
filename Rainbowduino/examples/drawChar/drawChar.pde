/*
 Rainbowduino v3.0 Library examples:

 Print Chars on 2D plane (8x8 matrix)

*/

#include <Rainbowduino.h>

void setup()
{
  Rb.init();
}

unsigned char x,y,z;

void loop()
{

  for(int i= 0x20; i<=0x7E; i++) //generates ASCII value of all printable characters
  {
     Rb.drawChar(i,0,1,random(0xFFFFFF)); 
     delay(500);
     Rb.blankDisplay();
  }
  
}




