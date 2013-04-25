// example of beaconsensor
#include <BeaconSensor.h>
#include <BeaconSensorDfs.h>
#include <Wire.h>

void setup()
{
    Serial.begin(38400);
    SENSOR.init(TEMPERATURESENSOR);
    Serial.println("init over");
}

unsigned char senDta[10];

void loop()
{
    int tmp = SENSOR.getSensor(senDta);

    for(int i = 0; i<tmp; i++)
    {
        Serial.print(senDta[i], DEC);
        Serial.print("\t");
    }

    Serial.println();

    delay(1000);
    
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/