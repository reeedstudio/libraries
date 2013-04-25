
#include <IRSendRev.h>

void setup()
{
    //enableIROut(38);
}


unsigned char d[] = {9, 90, 91, 11, 31, 4, 1, 2, 3, 4};
void loop()
{
    IR.Send(d, 38);
    delay(1000);
}