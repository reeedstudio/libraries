/*
  BeaconActuator.cpp
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "BeaconActuator.h"
#include "BeaconActuatorDfs.h"
#include <Wire.h>
#include <SeeedOLED.h>

/*********************************************************************************************************
** Function name: init
** Descriptions:  init actuator
*********************************************************************************************************/
void BeaconActuator::init(unsigned char id)
{
    idActuator = id;
    init_io();
}

/*********************************************************************************************************
** Function name: init_io
** Descriptions:  init actuator
*********************************************************************************************************/
void BeaconActuator::init_io()
{
    if(idActuator <= ACTUATORIOTOP)
    {
        pinMode(IOACTUATOR0, OUTPUT);
		digitalWrite(IOACTUATOR0, LOW);
    }
	else
	{
		switch(idActuator)
		{
			case ACTUATOROLED12864:
			Wire.begin();	
			SeeedOled.init();  //initialze SEEED OLED display
			delay(50);
			SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
			delay(50);
  			SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  			SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  			SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
			delay(50);
			break;
			
			default:
			;
		}
	}
    // other ... add code here
}

/*********************************************************************************************************
** Function name: driveActuator
** Descriptions:  dta[0]:datalen, data[1] -  data[datalen]
*********************************************************************************************************/
bool BeaconActuator::driveActuator(unsigned char *dta)
{
	unsigned int nTmp = 0;

    if(idActuator <= ACTUATORIOTOP && dta[0] == 1)
    {
        if(dta[1])
        {
            digitalWrite(IOACTUATOR0, HIGH);
        }
        else
        {
            digitalWrite(IOACTUATOR0, LOW);
        }
        return 1;
    }
	else
	{
		switch(idActuator)
		{
			case ACTUATOROLED12864:
			
			SeeedOled.setTextXY(0, 0);
			SeeedOled.putString("    ");
			SeeedOled.setTextXY(0, 0);
			if(dta[0] == 2)
			{
				nTmp = dta[1];
				nTmp <<= 8;
				nTmp += dta[2];
			//nTmp = 1023;
				SeeedOled.putNumber(nTmp);
			}
			else if(dta[0] == 1)
			{
				SeeedOled.putNumber(dta[1]);
			}
			
			break;
			
			default:
			;
		}
	}

    return 0;
}

/*********************************************************************************************************
** Function name: getSensorId
** Descriptions:  get sensor id
*********************************************************************************************************/
unsigned char BeaconActuator::getActuatorId()
{
    return idActuator;
}

BeaconActuator ACTUATOR;

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
