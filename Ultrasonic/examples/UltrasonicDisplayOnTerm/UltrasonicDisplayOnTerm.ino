#include <Ultrasonic.h>

Ultrasonic ultrasonic1(7);
Ultrasonic ultrasonic2(9);

void setup()
{
  
  Serial.begin(9600);
}

int val1 = 0;
int val2 = 0;

#define COMVAL  150

void loop()
{
  /*Serial.print("1:\t");
  ultrasonic1.MeasureInCentimeters();
  Serial.println(ultrasonic1.RangeInCentimeters);
  
  Serial.print("\r\n2:\t");
    ultrasonic2.MeasureInCentimeters();
  Serial.println(ultrasonic2.RangeInCentimeters);
  delay(500);*/
   // while(1)
  //  {
    ultrasonic1.MeasureInCentimeters();
    val1 = ultrasonic1.RangeInCentimeters;
    ultrasonic2.MeasureInCentimeters();
    val2 = ultrasonic2.RangeInCentimeters;
    Serial.println(val1);

    if(val1 < COMVAL)
    {
        delay(50);
        while(1)
        {
            ultrasonic1.MeasureInCentimeters();
            val1 = ultrasonic1.RangeInCentimeters;
            delay(20);
            if(val1 > COMVAL)break;
        }
        
        while(1)
        {
            ultrasonic2.MeasureInCentimeters();
            val2 = ultrasonic2.RangeInCentimeters;
            delay(20);
            if(val2 < COMVAL)break;
        }
        
        while(1)
        {
            ultrasonic2.MeasureInCentimeters();
            val2 = ultrasonic2.RangeInCentimeters;
            delay(20);
            if(val2 > COMVAL)break;
        }
        delay(200);
        Serial.println("some body in");
    }
    
    
    else if(val2 < COMVAL)
    {
        delay(50);
        while(1)
        {
            ultrasonic2.MeasureInCentimeters();
            val2 = ultrasonic1.RangeInCentimeters;
            delay(20);
            if(val2 > COMVAL)break;
            
        }
        
        while(1)
        {
            ultrasonic1.MeasureInCentimeters();
            val1 = ultrasonic1.RangeInCentimeters;
            delay(20);
            if(val1 < COMVAL)break;
        }
        
        while(1)
        {
            ultrasonic1.MeasureInCentimeters();
            val1 = ultrasonic1.RangeInCentimeters;
            delay(20);
            if(val1 > COMVAL)break;
        }
        delay(200);
        Serial.println("some body out");
    }

    
}
