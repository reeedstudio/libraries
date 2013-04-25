#include "Wifly.h"
#include <SoftwareSerial.h>
WiflyClass Wifly(2,3);

#define SSID1 (const char*)"Seeed-MKT"
#define PASSWORD (const char*)"seeed0728"
#define IP (const char*)"192.168.1.139"
#define PORT (const char*)"90"      

void setup()
{
    Serial.begin(9600);
    Wifly.init();
    Wifly.setConfig(SSID1,PASSWORD);
    Wifly.join(SSID1);
    Wifly.checkAssociated();
    while(!Wifly.connect(IP,PORT));
    Wifly.writeToSocket("Connected!");
}

int getAnalog()
{
    long sum = 0;
    for(int i = 0; i<50; i++)
    {
        sum += analogRead(A5);
    }
    return map(sum/50, 0, 986, 0, 1023);
}

// a = b return 1 else reutrn 0
bool cmp(int a, int b)
{
    if((a-b) > 2)return 0;
    if((b-a) < 2)return 0;
    return 1;
}

int sensor      = 0;
int sensorBuf   = 0;

void loop()
{
    char s[20] = "";
 /*   if(Wifly.canReadFromSocket())
    {
        Wifly.readFromSocket();
    }
    if(Serial.available())
    {
        Wifly.print((char)Serial.read());
    }
    */
    sensor = getAnalog();
    if(abs(sensor - sensorBuf)>2)
    {
        sensorBuf = sensor;
        sprintf(s, "sensor = %d\r\n", sensorBuf);
        Wifly.println(s);
        Serial.print(s);
        delay(100);
    }
    delay(10);
}
