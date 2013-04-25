#include <TimerOne.h>
#include <LightCom.h>
#include <BeaconDrive.h>
#include <Sleep_n0m1.h>

void TISR()
{
    LightCom1.TIMEISR();
    BcnDrive.ledIsr();
}

int len;
unsigned char sRev[10];


unsigned char sTst[] = {24, 42, 132, 35, 20, 62, 117, 33, 47, 38, 20, 63, 23, 43, 131, 36, 19, 62, 25, 41, 47, 37, 20, 63, 24, 59, 24, 42, 45, 36, 117};

void setup()
{
    BcnDrive.init();
    LightCom1.init(12);
    Timer1.initialize(ISRTIME);     // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
    Timer1.attachInterrupt(TISR);   // attach the service routine here

}

bool checkGoodDta(int len, unsigned char* dta)
{
    if(len == 2 && dta[0] == 0x51 && dta[1] == 0x01)return 1;
    return 0;
}

void loop()
{
    if(LightCom1.isData())
    {
        len = LightCom1.Recv(sRev);
        if(checkGoodDta(len, sRev))
        {
            BcnDrive.setLedShine(LEDCOLORGREEN, 500);
        }
        else
        {
            BcnDrive.setLedShine(LEDCOLORRED, 500);
        }
        
    }

    delay(1);
}

