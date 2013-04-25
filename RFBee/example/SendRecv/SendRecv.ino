// demo of rfbee send and recv 
// loovee 2013
#include <Arduino.h>
#include <EEPROM.h>
#include <RFBeeSendRev.h>
#include <RFBeeCore.h>

unsigned char dtaUart[100];             // serial data buf
unsigned char dtaUartLen = 0;           // serial data length

/*********************************************************************************************************
** Function name:           setup
** Descriptions:            setup
*********************************************************************************************************/
void setup(){

    pinMode(10, OUTPUT);
    RFBEE.init();
    Serial.begin(38400);
    Serial.println("ok");
}

unsigned char rxData1[200];               // data len
unsigned char len1;                       // len
unsigned char srcAddress1;
unsigned char destAddress1;
char rssi1;
unsigned char lqi1;
int result1;

unsigned char cntGetDta = 5;

/*********************************************************************************************************
** Function name:           loop
** Descriptions:            loop
*********************************************************************************************************/
void loop()
{
    if(RFBEE.isDta())                       // rfbee get data
    {
        result1 = receiveData(rxData1, &len1, &srcAddress1, &destAddress1, (unsigned char *)&rssi1 , &lqi1);
        for(int i = 0; i< len1; i++)
        {
            Serial.print(rxData1[i]);
        }
    }

    if(dtaUartLen)                          // serial get data
    {
        RFBEE.sendDta(dtaUartLen, dtaUart);
        dtaUartLen = 0;
    }

}

void serialEvent() 
{
    while (Serial.available()) 
    {
        dtaUart[dtaUartLen++] = (unsigned char)Serial.read();  
    }

}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/