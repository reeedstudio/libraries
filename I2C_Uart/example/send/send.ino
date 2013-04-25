// demo of I2C_Uart
#include <Wire.h>
#include <I2C_Uart.h>

char strSend[] = "01234567890123456789012345678901234567890123456789\n";
void setup()
{
    I2C_Uart_Init();
    putFloat(1.6523);
}

void loop()
{
    //I2C_Uart(strSend);
    delay(500);
}