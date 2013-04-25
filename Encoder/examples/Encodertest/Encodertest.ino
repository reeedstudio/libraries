#include <Encoder.h>
#include<TimerOne.h>
//Encoder encoder;
void setup()
{
  encoder.Timer_init();
  Serial.begin(9600);
}
void loop()
{
  if (encoder.rotate_flag ==1)
  {
    if (encoder.direct==0)
    { Serial.println("backward rotated!");}
     else
     {Serial.println("forward rotated!");}
    encoder.rotate_flag =0;
  }
 }
