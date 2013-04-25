/******************************************************************
 WizFi2x0 Shield test code for Arduino Uno
 
 Circuit:
 WizFi2x0 connected to Arduino via SPI
 
 RST: pin 2  // Output
 DRDY: pin 3  // Input
 CSB: pin 4  // output

 MOSI: pin 11  // output
 MISO: pin 12  // input
 SCK: pin 13  // out
 
 Created 27 Sep. 2012
 by James YS Kim  (jameskim@wiznet.co.kr, javakys@gmail.com)

*****************************************************************/

// WizFi210 communicates using SPI, so include the SPI library:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPI.h>
#include <WizFi2x0.h>
#include <WizFiClient.h>
#include <WizFiServer.h>
#include <TimerOne.h>

byte IsTimer1Expired = 0;
uint16_t CurrentTime = 0;

#define SSID 		"STEST"        // SSID of your AP
#define Key 	    "mmmmmmmm"    // Key or Passphrase
//#define Key 	        ""    // Key or Passphrase

unsigned char  SIP[4] 	        = {192, 168, 168, 201};
unsigned int ServerPort = 5000;
unsigned int SrcPort = 50000;

WizFi2x0Class myWizFi;
WizFiClient myClient[4];//(SIP, ServerPort);
WizFiServer myServer(SrcPort);
TimeoutClass ConnectInterval;

SPIChar spichar;

// pins used for the connection with the WizFi210

boolean Wifi_setup = false;
boolean ConnectionState = false;
boolean Disconnect_flag = false;
boolean Connect_flag = false;

///////////////////////////////
// 1msec Timer
void Timer1_ISR()
{
  uint8_t i;
  
  myWizFi.ReplyCheckTimer.CheckIsTimeout();
}
//
//////////////////////////////

void setup() {
  byte key, retval, i;
  byte retry_count = 0;
  byte tmpstr[64];
  
  Serial.begin(9600);
  Serial.println("\r\nSerial Init");
  
  for(i=0; i<4; i++)
    myClient[i] =  WizFiClient();
//  myServer = WizFiServer(5000);
  // initalize WizFi2x0 module:
  myWizFi.begin();
 
  ConnectInterval.init();

  // Socket Creation with Server IP address and Server Port num 
  
  // Timer1 Initialize
  Timer1.initialize(1000); // 1msec
  Timer1.attachInterrupt(Timer1_ISR);
 
  myWizFi.SendSync();
  myWizFi.ReplyCheckTimer.TimerStart(1000);
  
  Serial.println("Send Sync data");
  
  while(1)
  {
    if(myWizFi.CheckSyncReply())
    {
      myWizFi.ReplyCheckTimer.TimerStop();
      Serial.println("Rcvd Sync data");
      break;
    }
    if(myWizFi.ReplyCheckTimer.GetIsTimeout())
    {
      Serial.println("Rcving Sync Timeout!!");
      return;
    }
  }

  ////////////////////////////////////////////////////////////////////////////
  // AP association  
  while(1)
  {
    byte tmpstr[32];
    
    retval = myWizFi.associate(SSID, Key, NO_SECURITY, true);
    
    if(retval == 1){
      myWizFi.GetSrcIPAddr(tmpstr);
      Serial.println("WizFi2xo AP Associated");
      Serial.print("MY IPAddress: ");
      Serial.println((char *)tmpstr);
      Wifi_setup = true;
      break;
    }else{
      Serial.println("AP association Failed");
    }
  }
  
  if(myServer.begin())
    Serial.println("Server Listen OK");
  else
    Serial.println("Server Listen Failed");
}

void loop()
{
  uint8_t retval, i;
  byte rcvdBuf[129];
  byte cmd;
  byte TxBuf[100];
 
  memset(rcvdBuf, 0, 129);
  
  
  if(Wifi_setup)
  {  
    myWizFi.RcvPacket();
    for(i=0; i<4; i++)
    {
      if(myClient[i].available()){       
         if(myClient[i].read(rcvdBuf))
         {
           Serial.print("CID[");
           Serial.print((char)myClient[i].GetCID());
           Serial.print("]");
           Serial.println((char *)rcvdBuf);
           myClient[i].write(rcvdBuf);
         }
      }
    }
  }
}

void CheckConsoleInput(void)
{
  uint8_t ch;
  
  if(Serial.available() > 0)
    ch = Serial.read();
    
  switch(ch)
  {
  case 'd':
  case 'D':
    Disconnect_flag = true;
    break;
  case 'c':
  case 'C':
    Connect_flag = true;
    break;
  }
}
