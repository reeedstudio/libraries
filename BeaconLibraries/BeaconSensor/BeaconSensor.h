/*
  BeaconSensor.h
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2012-12-2
  
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

#ifndef __BEACONSENSOR_H__
#define __BEACONSENSOR_H__


class BeaconSensor{

    private:
    unsigned char idSensor;
    
    private:
    
    void init_io();
    
    //temperature%humidity sensor
    unsigned char getTempHumiDtaDo1();
    unsigned char getTempHumiDta(unsigned char *temphumi);
    
    // Barometer
    void bmp085GetTempPres(unsigned char *dta);
    char bmp085Read(unsigned char address);
    int bmp085ReadInt(unsigned char address);
    
    public:
    
    void init(unsigned char id);                        // init sensor, id: sensor id
    unsigned char getSensorId();                        // get id of sensor
    unsigned char getSensor(unsigned char *dta);        // get sensor data
    
};

extern BeaconSensor SENSOR;

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
