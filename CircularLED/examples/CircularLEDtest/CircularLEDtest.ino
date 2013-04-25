#include <CircularLED.h>
CircularLED circularLED;
unsigned int LED[24];
int k=0;
void setup() 
{
  Serial.begin(9600);
}
void loop() 
{
  for (int i =0;i<24;i++)
  {
    LED[i]=0;
  }
    LED[k]= 0xff;
    k++;
    if (k==24)
    k=0;
    
  circularLED.CircularLEDWrite(LED);
  delay(40);  
}
