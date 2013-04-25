//  CCx.cpp  Class to control the chipcon CCxxxx series transceivers
//  see http://focus.ti.com/lit/ds/symlink/cc1101.pdf for details on the CC1101

//  Copyright (c) 2010 Hans Klunder <hans.klunder (at) bigfoot.com>
//  Author: Hans Klunder, based on the original Rfbee v1.0 firmware by Seeedstudio
//  Version: May 22, 2010
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "RFBeeGlobals.h"

//---------- constructor ----------------------------------------------------

CCX::CCX(){}

// Power On Reset as described in  19.1.2 of cc1100 datasheet, tried APOR as described in 19.1.1 but that did not work :-(
void CCX::PowerOnStartUp()
{
    RF_SPI.mode((1 << SPR1) | (1 << SPR0));//SPICLK=CPU/64

    // start manual Power On Reset
    RF_SPI.slaveSelect(HIGH);
    delayMicroseconds(1);

    RF_SPI.slaveSelect(LOW);
    delayMicroseconds(10);

    RF_SPI.slaveSelect(HIGH);
    delayMicroseconds(41);

    RF_SPI.slaveSelect(LOW);

    // wait for MISO to go low
    while(digitalRead(MISO_PIN));

    RF_SPI.transfer(CCx_SRES);

    // wait for MISO to go low
    while(digitalRead(MISO_PIN));

    RF_SPI.slaveSelect(HIGH);

}

unsigned char CCX::Read(unsigned char addr,unsigned char* data)
{
    unsigned char result;

    RF_SPI.slaveSelect(LOW);
    // wait for MISO to go low
    while(digitalRead(MISO_PIN));

    result=RF_SPI.transfer(addr | 0x80);
    *data=RF_SPI.transfer(0);

    RF_SPI.slaveSelect(HIGH);
    return result;
}

unsigned char CCX::ReadBurst(unsigned char addr, unsigned char* dataPtr, unsigned char size)
{

    unsigned char result;

    RF_SPI.slaveSelect(LOW);
    // wait for MISO to go low
    while(digitalRead(MISO_PIN));

    result=RF_SPI.transfer(addr | 0xc0);

    while(size)
    {
        *dataPtr++ = RF_SPI.transfer(0);
        size--;
    }

    RF_SPI.slaveSelect(HIGH);

    return result;
}

unsigned char CCX::Write(unsigned char addr, unsigned char dat)
{
    unsigned char result;

    RF_SPI.slaveSelect(LOW);
    // wait for MISO to go low
    while(digitalRead(MISO_PIN));

    result=RF_SPI.transfer(addr);
    result=RF_SPI.transfer(dat);

    RF_SPI.slaveSelect(HIGH);

    return result;
}

unsigned char CCX::WriteBurst(unsigned char addr, const unsigned char* dataPtr, unsigned char size)
{

    unsigned char result;

    RF_SPI.slaveSelect(LOW);
    // wait for MISO to go low
    while(digitalRead(MISO_PIN));

    result=RF_SPI.transfer(addr | 0x40);

    while(size)
    {
        result = RF_SPI.transfer(*dataPtr++);
        size--;
    }

    RF_SPI.slaveSelect(HIGH);

    return result;
}

unsigned char CCX::Strobe(unsigned char addr)
{
    unsigned char result;

    RF_SPI.slaveSelect(LOW);
    // wait for MISO to go low
    while(digitalRead(MISO_PIN));

    result=RF_SPI.transfer(addr);

    RF_SPI.slaveSelect(HIGH);

    return result;
}

//configure registers of cc1100 making it work in specific mode
void CCX::Setup(unsigned char configId)
{
    unsigned char reg;
    unsigned char val;
    if (configId < CCX_NR_OF_CONFIGS)
    for(unsigned char i = 0; i< CCX_NR_OF_REGISTERS; i++){
        reg=pgm_read_byte(&CCx_registers[i]);
        val=pgm_read_byte(&CCx_registerSettings[configId][i]);//read flash data no problem
        unsigned char temp = Write(reg,val);
    }
}

void CCX::setPA(unsigned char configId,unsigned char paIndex)
{
    unsigned char PAval=pgm_read_byte(&CCx_paTable[configId][paIndex]);
    CCx.Write(CCx_PATABLE,PAval);
}


void CCX::Mode(unsigned char md){

}

unsigned char CCX::NrOfConfigs(){
    return CCX_NR_OF_CONFIGS;
}

unsigned char CCX::RSSIdecode(unsigned char rssiEnc){
    unsigned char rssi;
    unsigned char rssiOffset=74;  // is actually dataRate dependant, but for simplicity assumed to be fixed.

    // RSSI is coded as 2's complement see section 17.3 RSSI of the cc1100 datasheet
    if (rssiEnc >= 128)
    rssi = (( rssiEnc - 256) >> 1) - rssiOffset;
    else
    rssi = (rssiEnc >> 1) - rssiOffset;
    return rssi;
}

//---------- preinstantiate CCx object --------------------------------------

CCX CCx = CCX();

