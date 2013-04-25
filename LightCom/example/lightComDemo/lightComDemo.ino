#include <TimerOne.h>
#include <LightCom.h>
#include <I2C_Uart.h>
#include <Wire.h>

void TISR()
{
    LightCom1.TIMEISR();
}

int len;
unsigned char sRev[10];

unsigned char dta    = 0;
unsigned char dtaBuf = 0;


unsigned char sTst[] = {24, 42, 132, 35, 20, 62, 117, 33, 47, 38, 20, 63, 23, 43, 131, 36, 19, 62, 25, 41, 47, 37, 20, 63, 24, 59, 24, 42, 45, 36, 117};
void setup()
{
#if __BEACON
    I2C_Uart_Init(); 
#else
    Serial.begin(38400);
#endif

#if __BEACON
    LightCom1.init(12);
#else
    LightCom1.init(5);
#endif

    Timer1.initialize(ISRTIME);     // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
    Timer1.attachInterrupt(TISR);   // attach the service routine here
    
    __PRINTLN("init over");

}

int cntAll      = 0;
int cntRight    = 0;
float rate      = 0.0;

unsigned char stmp[10] = {15, 58, 254, 95, 00, 39, 175, 96, 59, 49};
unsigned char errCnt = 0;

void loop()
{
    if(LightCom1.isData())
    {
        len = LightCom1.Recv(sRev);

        __PRINT("len = ");__PRINTLN(len);
        for(int i = 0; i<len; i++)
        {
            __PRINT(sRev[i]);__PRINT("\t");
        }
        __PRINTLN();
    }

    delay(1);
}

