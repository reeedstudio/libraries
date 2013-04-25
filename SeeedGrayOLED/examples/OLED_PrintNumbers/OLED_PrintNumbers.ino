#include <Wire.h>
#include <SeeedGrayOLED.h>


void setup()
{
    Serial.begin(38400);
    
    Wire.begin();
    SeeedGrayOled.init();  //initialize SEEED Gray OLED display

    SeeedGrayOled.clearDisplay();           //clear the screen and set start position to top left corner
    SeeedGrayOled.setNormalDisplay();       //Set display to Normal mode
    SeeedGrayOled.setVerticalMode();        // Set to vertical mode for displaying text

    Serial.println("init over");
    
    SeeedGrayOled.setTextXY(0,0);           //Set the cursor to 0th line, 0th Column
    SeeedGrayOled.putNumber(123);           //Print number
    SeeedGrayOled.setTextXY(1,0);           //Set the cursor to 1st line, 0th Column
    SeeedGrayOled.putNumber(0xFFFF);        //Print number
    SeeedGrayOled.setTextXY(2,0);           //Set the cursor to 2nd line, 0th Column
    SeeedGrayOled.putNumber(0xFFFFFFFF);    //Print number
    SeeedGrayOled.setTextXY(3,0);           //Set the cursor to 3rd line, 0th Column
    SeeedGrayOled.putFloat(5.1061234567, 10);        //Print number

}

void loop()
{

}


