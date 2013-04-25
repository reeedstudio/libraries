// demo of rfbee send and recv
#include <Arduino.h>
#include <EEPROM.h>
#include <RFBeeSendRev.h>
#include <RFBeeCore.h>
#include <Sleep_n0m1.h>
#include <Power.h>
#include <Sleep_n0m1.h>
#include <TimerOne.h>

#define FRAMESTART1                 0x53                // data frame start1
#define FRAMESTART2                 0x01                // data frame start2
#define FRAMEEND1                   0x2f                // data frame end1
#define FRAMEEND2                   0x45                // data frame end2

#define __DebugRf           0
#define HTCHECK             1

#if __DebugRf
#define __print(X) Serial.print(X)
#define __println(X) Serial.println(X)
#else
#define __print(X)
#define __println(X)
#endif

#define SPICSHIGH()         digitalWrite(10, HIGH)
#define SPICSLOW()          digitalWrite(10, LOW)

unsigned char dtaUart[100];
unsigned char dtaUartLen = 0;
bool stringComplete      = false;        // if get data

Sleep sleep;

/*********************************************************************************************************
** Function name:           pwrDown
** Descriptions:            power down mode, for tSleep ms
*********************************************************************************************************/
void pwrDown(unsigned long tSleep)
{
    unsigned char regTmp;
    CCx.Read(0x18, &regTmp);
    CCx.Write(0x18, regTmp|0x01);
    CCx.Strobe(CCx_SIDLE);              // IDLE
    CCx.Strobe(CCx_SFRX);               // Flush RX buffer
    CCx.Strobe(CCx_SPWD);               // Standby   
    SPICSHIGH();   
    power_adc_disable();
    power_twi_disable();
    sleep.pwrDownMode();                //set sleep mode
    sleep.sleepDelay(tSleep);           //sleep for: sleepTime
}

/*********************************************************************************************************
** Function name:           wakeUp
** Descriptions:            wake up
*********************************************************************************************************/
void wakeUp()
{
    power_adc_enable();
    power_twi_enable();
    SPICSLOW(); 
    RFBEE.init();
}

/*********************************************************************************************************
** Function name:           checkGoodDta
** Descriptions:            if uart get good data
*********************************************************************************************************/
unsigned char checkGoodDta(unsigned char *dta)
{
#if HTCHECK

    __println("BEGIN TO CHECK GOOD DATA!!");
    
    for(int i = 0; i < dtaUartLen; i++)
    {
        __print(dta[i]);
        __print("\t");
    }
    __println();
    
    int ioffset = 0;
    
    for(ioffset = 0; ioffset < dtaUartLen; ioffset++)
    {
        if(dta[ioffset] == FRAMESTART1 && dta[ioffset+1] == FRAMESTART2)
        {
            break;
        }
    }
    
    if(dtaUartLen - ioffset < 6)
    {
        __println("get bad data");
        return 0;
    }

    dtaUartLen -= ioffset;
    
    //dta = &dta[ioffset];
    
    for(int i = 0; ;i++)
    {
        dta[i] = dta[i+ioffset];
        __print(dta[i]);
        __print("\t");
        if(dta[i-1] == FRAMEEND1 && dta[i] == FRAMEEND2)
        {
            break;
        }
    }
    __println();
    __println("get good data");
    
    return dtaUartLen;
#else
    return 1;
#endif
}

unsigned char isSleep()
{
    if(dtaUart[2] == 0x55 && dtaUart[3] == 0x55 && dtaUart[4] == 0x55)
    {
        __print("ATSLEEP: ");
        __println(900);
#if __DebugRf
        delay(20);
#endif
        pwrDown(900);
        wakeUp();
        __println("awake");
        return 1;
    }
    return 0;
}
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
    if(RFBEE.isDta())
    {
        result1 = receiveData(rxData1, &len1, &srcAddress1, &destAddress1, (unsigned char *)&rssi1 , &lqi1);
        for(int i = 0; i< len1; i++)
        {
            Serial.write(rxData1[i]);
        }
    }

#if HTCHECK
    if(stringComplete) 
    {
        
        if(checkGoodDta(dtaUart))
        {

            if(isSleep() == 0)
            {
                RFBEE.sendDta(dtaUartLen, dtaUart);
            }
        }
        dtaUartLen      = 0;
        stringComplete  = 0;
    }
#else
    if(dtaUartLen)
    {
        if(isSleep() == 0)
        {
            RFBEE.sendDta(dtaUartLen, dtaUart);
        }
        dtaUartLen = 0;
    }
#endif
}

void serialEvent() 
{
#if HTCHECK
    while (Serial.available()) 
    {
        dtaUart[dtaUartLen++] = (unsigned char)Serial.read();  
        if(dtaUart[dtaUartLen-1] == FRAMEEND2 && dtaUart[dtaUartLen-2] == FRAMEEND1)
        {
            stringComplete = 1;
        }
        
        if(dtaUartLen > 80)
        {
            dtaUartLen      = 0;
            stringComplete  = 0;
        }
    }
#else
    while (Serial.available()) 
    {
        dtaUart[dtaUartLen++] = (unsigned char)Serial.read();  
    }
#endif
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/