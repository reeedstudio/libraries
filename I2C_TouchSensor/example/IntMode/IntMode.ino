// I2C Touch Sensor Int Mode

#include <Wire.h> // include I2C library
#include <I2C_TouchSensor.h>
#include <MPR121.h>

I2C_TouchSensor touchsensor;            // keep track of 4 pads' states

void setup() {

    Wire.begin();                       // needed by the GroveMultiTouch lib
    touchsensor.initialize();           // initialize the feelers     // initialize the containers
    attachInterrupt(0, isr, FALLING);   // use interrupt 0
    Serial.begin(38400);
}

int flg = 0;

void isr()
{
    detachInterrupt(0);       
    Serial.println("get in int");
    
    flg = 1;                            // get some touch, set flg
    attachInterrupt(0, isr, FALLING);
  
}
void loop(){
  // read the state of the pushbutton value:
  
  if(flg)                               // if touch
  {
    flg = 0;                            // clear flg
    for(int i = 0; i<8; i++)
    if (touchsensor.isTouch(i))         // check pin i
    {
        Serial.print("pin ");
        Serial.print(i);
        Serial.println(" was  touched");
    }
  }
}


