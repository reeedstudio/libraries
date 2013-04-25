/*
  Queue.cpp
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

#include "BeaconSensor.h"
#include "BeaconSensorDfs.h"

#include <Wire.h>
/*********************************************************************************************************
** Function name: init
** Descriptions:  init sensor id
*********************************************************************************************************/
void BeaconSensor::init(unsigned char id)
{
    idSensor = id;
    init_io();
}

/*********************************************************************************************************
** Function name: init_io
** Descriptions:  init io
*********************************************************************************************************/
void BeaconSensor::init_io()
{

    // if idSensor <= SENSORANALOGTOP , then it is an analog(IO) sensor, 
    // need not to init
    
    if(idSensor == TEMPANDHUMI_TEMP)             // temperature&humidity sensor
    {
        pinMode(IOSENSOR0, OUTPUT);
    }
	
    else if(idSensor == BAROMETERSENSOR)
    {
        Wire.begin();
    }

    // some others : add code here
}

/*********************************************************************************************************
** Function name: getSensorId
** Descriptions:  get sensor id
*********************************************************************************************************/
unsigned char BeaconSensor::getSensorId()
{
    return idSensor;
}

/*********************************************************************************************************
** Function name: getSensor
** Descriptions:  get sensor data: dta[0]: datalen dta[1]-dta[n], data ;return : dta[0]+1
*********************************************************************************************************/
unsigned char BeaconSensor::getSensor(unsigned char *dta)
{
    
    if(idSensor == TEMPANDHUMI_TEMP)             // temperature&humidity sensor
    {
    	unsigned char dtaTmp[5];
        if(getTempHumiDta(dtaTmp))                  // get bad data
        {
        	dta[0] = 1;
			dta[1] = dtaTmp[1];
            return 2;
        }
        else 
        {
            return 0;
        }
    }
	else if(idSensor == TEMPERATURESENSOR)		// grove temperature
	{
		int a=analogRead(0);
  		float resistance=(float)(1023-a)*10000/a; //get the resistance of the sensor;
  		dta[1] = 1/(log(resistance/10000)/3975+1/298.15)-273.15;//convert to temperature via datasheet ;
  		dta[0] = 1;
		return 2;
	}
    
    else if(idSensor == BAROMETERSENSOR)
    {
        bmp085GetTempPres(dta);
        return 6;
    }
	else if(idSensor <= SENSORANALOGTOP)         // analog & IO
    {
        unsigned int tmp = 0;

		for(int i = 0;i<10; i++)
		{
			tmp += analogRead(IOSENSOR0);
		}
		tmp /= 10;
		
        dta[0] = 2;
        dta[1] = tmp>>8;
        dta[2] = tmp;
        return 3;
    }
    
    return 0;
}

/*********************************************************************************************************
** Function name: getTempHumiDtaDo1
** Descriptions:  use by getTempHumiDta
*********************************************************************************************************/
unsigned char BeaconSensor::getTempHumiDtaDo1()
{
    unsigned char i = 0;
    unsigned char result=0;
    for(i=0; i< 8; i++)
    {
        while(!digitalRead(IOSENSOR0));  // wait for 50us
        delayMicroseconds(30);
        if(digitalRead(IOSENSOR0))
        result |=(1<<(7-i));
        while(digitalRead(IOSENSOR0));  // wait '1' finish
    }
    return result;
}

/*********************************************************************************************************
** Function name: getTempHumiDta
** Descriptions:  get temperature and humi, return 1:ok, return 0, err
*********************************************************************************************************/
unsigned char BeaconSensor::getTempHumiDta(unsigned char *temphumi)
{
    unsigned char dht11_dat[5];
    unsigned char i;

    digitalWrite(IOSENSOR0,LOW);
    delay(18);

    digitalWrite(IOSENSOR0,HIGH);
    delayMicroseconds(40);

    pinMode(IOSENSOR0,INPUT);

    while(digitalRead(IOSENSOR0)){
        return 0;
    }
    delayMicroseconds(80);

    while(!digitalRead(IOSENSOR0)){
        return 0;
    }

    delayMicroseconds(80);
    // now ready for data reception
    for (i=0; i<5; i++)
    dht11_dat[i] = getTempHumiDtaDo1();

    pinMode(IOSENSOR0,OUTPUT);
    digitalWrite(IOSENSOR0,HIGH);

    unsigned char dht11_check_sum = dht11_dat[0]+dht11_dat[1]+dht11_dat[2]+dht11_dat[3];
    // check check_sum
    if(dht11_dat[4] != dht11_check_sum)
    {
        return 0;
    }
    
    temphumi[0] = 4;
    temphumi[1] = (dht11_dat[2]*256+dht11_dat[3])/10;
    temphumi[2] = (dht11_dat[2]*256+dht11_dat[3])%10;

    temphumi[3] = (dht11_dat[0]*256+dht11_dat[1])/10;
    temphumi[4] = (dht11_dat[2]*256+dht11_dat[3])%10;
    return 1;

}

/*********************************************************************************************************
** Function name: bmp085GetTempPres
** Descriptions:  barometer get data
*********************************************************************************************************/
void BeaconSensor::bmp085GetTempPres(unsigned char *dta)
{
    dta[0]  =  5;
    long b5;
    long x1, x2;
    unsigned int ac5 = bmp085ReadInt(0xB2);
    int ac6 = bmp085ReadInt(0xB4);
    int mc = bmp085ReadInt(0xBC);
    int md = bmp085ReadInt(0xBE);

    // make ut
        unsigned int ut;
    // Write 0x2E into Register 0xF4
    // This requests a temperature reading
    Wire.beginTransmission(BAROMETERADDRESS);
    Wire.write(0xF4);
    Wire.write(0x2E);
    Wire.endTransmission();
    // Wait at least 4.5ms
    delay(5);
    // Read two bytes from registers 0xF6 and 0xF7
    ut = bmp085ReadInt(0xF6);

    x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
    x2 = ((long)mc << 11)/(x1 + md);
    b5 = (x1 + x2);
    
    //*temp = (float)(( b5+8)>>4)/10.0;
    
    dta[4] = ((b5+8)>>4)/10;
    dta[5] = ((b5+8)>>4)%10;
    
    long x3, b3, b6, p;
    unsigned long b4, b7;

    int ac1 = bmp085ReadInt(0xAA);
    int ac2 = bmp085ReadInt(0xAC);
    int ac3 = bmp085ReadInt(0xAE);
    unsigned int ac4 = bmp085ReadInt(0xB0);
    int b1 = bmp085ReadInt(0xB6);
    int b2 = bmp085ReadInt(0xB8);

    b6 =  b5-4000;
    // Calculate B3
    x1 = (b2 * (b6 * b6)>>12)>>11;
    x2 = (ac2 * b6)>>11;
    x3 = x1 + x2;
    b3 = (((((long)ac1)*4 + x3)<<0) + 2)>>2;

    // Calculate B4
    x1 = (ac3 * b6)>>13;
    x2 = (b1 * ((b6 * b6)>>12))>>16;
    x3 = ((x1 + x2) + 2)>>2;
    b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;
    
    unsigned char msb, lsb, xlsb;
    unsigned long up = 0;
    // Request a pressure reading w/ oversampling setting
    Wire.beginTransmission(BAROMETERADDRESS);
    Wire.write(0xF4);
    Wire.write(0x34 + (0<<6));
    Wire.endTransmission();
    // Wait for conversion, delay time dependent on 0
    delay(2 + (3<<0));
    // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
    msb     = bmp085Read(0xF6);
    lsb     = bmp085Read(0xF7);
    xlsb    = bmp085Read(0xF8);
    up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-0);

    b7 = ((unsigned long)(up - b3) * (50000>>0));
    
    p = (b7<0x80000000) ? (b7<<1)/b4 : (b7/b4)<<1;
    x1 = (p>>8) * (p>>8);
    x1 = (x1 * 3038)>>16;
    x2 = (-7357 * p)>>16;
    p += (x1 + x2 + 3791)>>4;
   // *pres = p;
   
    dta[1] = p/10000;
    dta[2] = (p%10000)/100;
    dta[3] = p%100;  
    
}

/*********************************************************************************************************
** Function name: bmp085Read
** Descriptions:  use by bmp085GetTempPres
*********************************************************************************************************/
char BeaconSensor::bmp085Read(unsigned char address)
{
    unsigned char data;
    Wire.beginTransmission(BAROMETERADDRESS);
    Wire.write(address);
    Wire.endTransmission();
    
    Wire.requestFrom(BAROMETERADDRESS, 1);
    while(!Wire.available());
    return Wire.read();
}

/*********************************************************************************************************
** Function name: bmp085ReadInt
** Descriptions:  use by bmp085GetTempPres
*********************************************************************************************************/
int BeaconSensor:: bmp085ReadInt(unsigned char address)
{
    unsigned char msb, lsb;
    Wire.beginTransmission(BAROMETERADDRESS);
    Wire.write(address);
    Wire.endTransmission();
    Wire.requestFrom(BAROMETERADDRESS, 2);
    while(Wire.available()<2);
    msb = Wire.read();
    lsb = Wire.read();
    return (int) msb<<8 | lsb;
}
    
BeaconSensor SENSOR;

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
