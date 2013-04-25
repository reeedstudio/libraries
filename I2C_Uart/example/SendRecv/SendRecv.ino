// demo of I2C_Uart
#include <Wire.h>
#include <I2C_Uart.h>

char uartDta[50];
int uartDtaLen      = 0;
bool getComplete    = 0;

void setup()
{
    I2C_Uart_Init();
    Serial.begin(38400);

}

void loop()
{
    delay(1);
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
 
    Serial.print(inChar);
  }
}
