// cloud beacon prototype code
// loovee 2013-3-22
#include <SoftwareSerial.h>
#include <WiFly.h>
#include <Wire.h>
#include <TimerOne.h>
#include "HTTPClient.h"

#define _CLOUD          1
#define _CLOUDSEND      0

#define SSID      "STEST"
#define KEY       "mmmmmmmm"
#define AUTH      WIFLY_AUTH_WPA2_PSK


#define HTTP_POST_URL_CLOUD "http://api.yeelink.net/v1.0/device/2266/sensor/2960/datapoints"
#define HTTP_POST_URL_ATOM "http://api.yeelink.net/v1.0/device/2266/sensor/2959/datapoints"
#define HTTP_POST_HEADERS "U-ApiKey: 61edb2da67b94b575acd53b83e72c852\r\n"

#if _CLOUD

#define NDEVICE     2

#define SENDATOM    0
#define SENDCLOUD   1

#define START1      0x53
#define START2      0x03
#define END1        0x2F
#define END2        0x45

#endif
// Pins' connection
// Arduino       WiFly
//  2    <---->    TX
//  3    <---->    RX

WiFly wifly(2, 3);
HTTPClient http;

#if _CLOUD

unsigned char wireDta[20];
unsigned char wireLen           = 0;
unsigned char wireDtaComplete   = 0;

unsigned long sendType  = 0;            // which to send to yeelink

bool flgGetAtom         = 0;            // if get Atom data
bool flgGetCloud        = 0;            // if get cloud data
bool flgYeeBsy          = 0;            // if yeelink busy

unsigned int  dtaAtom   = 0;            // latest Atom data
unsigned int  dtaCloud  = 0;            // latest Cloud data

unsigned long start_millis = 0;
char post_data_buf[32];
unsigned long cnt = 0;

#endif
/*********************************************************************************************************
** Function name: sendDtaToYeelink
** Descriptions:  send data to yeelink
*********************************************************************************************************/
int sendDtaToYeelink(unsigned char type)
{
#if _CLOUD
    flgYeeBsy   = 1;            // busy again
    cnt         = 0;            // clear count
    
    unsigned int  dtaVal[2] = {dtaAtom, dtaCloud};
    char postUrl[2][100]    = {HTTP_POST_URL_ATOM, HTTP_POST_URL_CLOUD};
    
    snprintf(post_data_buf, sizeof(post_data_buf), "{\"value\": %d}\r\n", dtaVal[type]);
    
    while(http.post(postUrl[type], HTTP_POST_HEADERS, post_data_buf, 10000) < 0);

    char r;
    while (wifly.receive((uint8_t *)&r, 1, 1000) == 1) 
    {
        Serial.print(r);
    }
#else

    return 0;
#endif
}

/*********************************************************************************************************
** Function name: timerIsr
** Descriptions:  timerIsr
*********************************************************************************************************/
#if _CLOUD
void timerIsr()
{
    cnt++;
    if(cnt > 120)
    {
        flgYeeBsy = 0;
    }
}
#endif
/*********************************************************************************************************
** Function name: dtaProc
** Descriptions:  dtaProc
*********************************************************************************************************/
unsigned char dtaProc()
{
#if _CLOUD
    int offset = 0;
    for(offset = 0; offset<wireLen; offset++)
    {
        if(wireDta[offset] == START1 && wireDta[offset+1] == START2)
        {
            break;
        }
    }
    
    Serial.print("offset = ");Serial.println(offset);
    if(offset > 6)return 0;
    
    if(wireDta[offset+2] == SENDATOM)
    {
        flgGetAtom  = 1;
        dtaAtom     = wireDta[offset+3];
        dtaAtom     = dtaAtom<<8;
        dtaAtom    += wireDta[offset+4];        
        Serial.print("dtaAtom = ");Serial.println(dtaAtom);
    }
    else if(wireDta[offset+2] == SENDCLOUD)
    {
        flgGetCloud  = 1;
        dtaCloud    = wireDta[offset+3];
        dtaCloud    = dtaCloud<<8;
        dtaCloud   += wireDta[offset+4];  
        Serial.print("dtaCloud = ");Serial.println(dtaCloud);
    }
    
    return 1;
#else
    return 0;
#endif
}

/*********************************************************************************************************
** Function name: setup
** Descriptions:  setup
*********************************************************************************************************/
void setup() 
{
    Serial.begin(9600);
    wifly.reset();
    while (1) 
    {
        Serial.println("Try to join " SSID );
        if (wifly.join(SSID, KEY, AUTH))
        {
            Serial.println("Succeed to join "SSID);
            wifly.clear();
            break;
        } 
        else 
        {
            Serial.println("Failed to join "SSID);
            //Serial.println("Wait 1 second and try again...");
            delay(1000);
        }
    }

    Wire.begin(4);                      // join i2c bus with address #4
    Wire.onReceive(receiveEvent);       // register event
 
    Timer1.initialize(100000);          // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
    Timer1.attachInterrupt( timerIsr ); // attach the service routine here
    
    Serial.println("init over");
}

/*********************************************************************************************************
** Function name: loop
** Descriptions:  loop
*********************************************************************************************************/
void loop() 
{

    if(cnt == 120)
    {
        Serial.println("YEELINK READY!");
        cnt++;
    }
    if(wireDtaComplete)
    {
        Serial.println("GET DATA FROM I2C:");
        for(int i = 0; i<wireLen; i++)
        {
            Serial.print(wireDta[i]);
            Serial.print("\t");
        }
        
        Serial.println();
        dtaProc();
        wireDtaComplete = 0;
        wireLen         = 0;
    }
    
    if(!flgYeeBsy && (flgGetAtom || flgGetCloud))
    {
        //Serial.println("YEELINK NOT BUSY!");
        Serial.println("BEGIN TO SEND DTA TO YEELINK");
        if(sendType == SENDATOM)
        {

            if(flgGetAtom)                      // get Atom data
            {
                flgGetAtom = 0;
                sendDtaToYeelink(SENDATOM);
                //Serial.println("SEND ATOM DATA TO YEELINK");
                sendType = 1-sendType;
            }
            else
            {
                flgGetCloud = 0;
                sendDtaToYeelink(SENDCLOUD);
                //Serial.println("SEND CLOUD DATA TO YEELINK");
            }
        }
        else
        {
            if(flgGetCloud)
            {
                flgGetCloud = 0;
                sendDtaToYeelink(SENDCLOUD);
                sendType = 1-sendType;
            }
            else
            {
                flgGetAtom = 0;
                sendDtaToYeelink(SENDATOM);
            }
        }
    }

}

/*********************************************************************************************************
** Function name: setup
** Descriptions:  setup
*********************************************************************************************************/
#if _CLOUD
void receiveEvent(int howMany)
{
    while(0 < Wire.available()) // loop through all but the last
    {
        wireDta[wireLen++] = Wire.read(); // receive byte as a character
        if(wireDta[wireLen-1] == END2 && wireDta[wireLen-2] == END1)
        {
            wireDtaComplete = 1;
        }
    }
}
#endif