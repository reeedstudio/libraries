// example of beaconActuator
#include <BeaconActuator.h>
#include <BeaconActuatorDfs.h>
#include <TM1637.h>
#include <Wire.h>
#include <SeeedOLED.h>

unsigned char dtaAc[5] = {4, 1, 2, 3, 5};

void setup()
{
    //Serial.begin(38400);
    ACTUATOR.init(ACTUATOROLED12864);
   // Serial.println("init over");
    //ACTUATOR.driveActuator(dtaAc);
	//ACTUATOR.driveActuator(dtaAc);
}

void loop()
{

    for(int i = 0; i<100; i++)
	{
		if(i<10)
		{
			dtaAc[0] = 1;
			dtaAc[1] = i;
		}
		else if(i<100)
		{
			dtaAc[0] = 2;
			dtaAc[1] = i/10;
			dtaAc[2] = i%10;
		}
		ACTUATOR.driveActuator(dtaAc);
		delay(100);
	}

}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/