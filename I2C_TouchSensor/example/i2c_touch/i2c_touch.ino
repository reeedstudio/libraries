/* GroveI2CTouchTest.pde - Sample code for the SeeedStudio Grove I2C Touch Sensor
http://seeedstudio.com/wiki/index.php?title=Twig_-_I2C_Touch_Sensor_v0.93b
Prerequisite: A modification of the Grove I2C Touch sensor is needed Solder a pin to
the INT terminal in the I2C sensor to connect it to one pin in Arduino  Created by
Wendell A. Capili, August 27, 2011. http://wendellinfinity.wordpress.com
Released into the public domain.*/

#include <Wire.h> // include I2C library
#include <I2C_TouchSensor.h>
#include <MPR121.h>

I2C_TouchSensor touchsensor;         // keep track of 4 pads' states

void setup()
{
    Serial.begin(38400);            // for debugging
    Wire.begin();                   // needed by the GroveMultiTouch lib
    touchsensor.initialize();       // initialize the feelers     // initialize the containers
}

void loop()
{

    for (int i=0;i<8;i++)
    {
        if (touchsensor.isTouch(i))     // check pin i
        {
            Serial.print("pin ");
            Serial.print(i);
            Serial.println(" was  touched");
        }
    }
    
    delay(100);         // check per 100ms
}
