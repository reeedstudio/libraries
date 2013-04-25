#include <TimerOne.h>
#include <LightCom.h>
#include <I2C_Uart.h>
#include <Wire.h>

unsigned char ledState = 0;
long ledCnt = 0;

void TISR()
{
    ledCnt++;
    LightCom1.TIMEISR();
    digitalWrite(13, ledState);
    
    if(1 == ledCnt % 20)
    ledState = 1 - ledState;
    
}

int len;
unsigned char sRev[10];

unsigned char dta    = 0;
unsigned char dtaBuf = 0;

void beep()
{
    digitalWrite(3, HIGH);
    delay(150);
    digitalWrite(3, LOW);
}

void setup()
{
    pinMode(3, OUTPUT);
#if __BEACON
    I2C_Uart_Init();
#else
    Serial.begin(38400);
#endif
    pinMode(13, OUTPUT);
    LightCom1.init(12);
    Timer1.initialize(1000);     // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
    Timer1.attachInterrupt(TISR);   // attach the service routine here
    //__PRINTLN("init over");
    
    for(int i = 0;i<1; i++)
    {
        beep();delay(500);
    }

}

int testRight = 0;
int testAll   = 0;
float rate    = 0.0;
void loop()
{
    if(LightCom1.isData())
    {
        Timer1.stop();
        len = LightCom1.Recv(sRev);
        __PRINT("len = ");__PRINTLN(len);
        for(int i = 0; i<len; i++)
        {
            __PRINT(sRev[i]);__PRINT("\t");
        }
        __PRINTLN();
        Timer1.initialize(1000);        // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
        Timer1.attachInterrupt(TISR);   // attach the service routine here
        //Timer1.start();
        
        if(90 == sRev[0])
        {
            testRight++;
        }
        else
        {
            beep();
        }
        testAll++;
        rate = (float)testRight*1.0/(float)testAll*1.0;
        rate = rate*100;
        
        __PRINT("TEST:  ");__PRINTLN(testAll);
        __PRINT("RIGHT: ");__PRINTLN(testRight);
        __PRINT("RATE:  ");__PRINT(rate);__PRINTLN("%");
    }

    delay(1);
}

