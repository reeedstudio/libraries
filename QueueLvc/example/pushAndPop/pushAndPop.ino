// example of queue
#include <Queue.h>

unsigned char ss[50];
void ssinit()
{
    for(int i = 0; i < 10; i++)
    {
        ss[i] = i;
    }
}

void setup()
{
    Serial.begin(38400);
    Serial.println("init over");
    ssinit();
    Queue.init();
    
    Queue.dispHeadCnt();
    Queue.dispQueue();
    
    Serial.println("push 5 dta");
    for(int i = 0; i<5; i++)
    {
        Queue.push(ss[i]);
        Queue.dispHeadCnt();
    }
    
    Serial.println("pop 5 dta");
    for(int i = 0; i<5; i++)
    {
        Queue.pop();
        Queue.dispHeadCnt();
    }
    
    Serial.println("push 12 dta");
    for(int i = 0; i<12; i++)
    {
        Queue.push(ss[i]);
        Queue.dispHeadCnt();
    }

}

void loop()
{
    delay(100);
}
