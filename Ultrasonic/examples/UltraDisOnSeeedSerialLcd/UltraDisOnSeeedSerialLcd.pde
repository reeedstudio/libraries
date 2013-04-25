//this is democode for ultrasonic ranger and display the 
//result on seeedstudio serialLcd. make sure you installed the serialLCD /
// NewSofeSerial and Ultrasonic library 
//before your download the firmware to your arduino/seeeduino.you can 
//also find more information on the seeedstudio serialLCD at :
//http://www.seeedstudio.com/wiki/Twig_-_Serial_LCD_v1.0b

#include <NewSoftSerial.h>

#include <SerialLCD.h>

#include <Ultrasonic.h>

SerialLCD slcd(11,12);
Ultrasonic ultrasonic(7);
 void setup(){
   slcd.begin();
 //  slcd.print("The distance:");
}
void loop()
{
  ultrasonic.MeasureInCentimeters();
  delay(150);
  slcd.clear();
  slcd.setCursor(0,0);
  slcd.print("The distance:");
  slcd.setCursor(0,1) ;
  slcd.print(ultrasonic.RangeInCentimeters,DEC);
  slcd.setCursor(5,1) ;  
  slcd.print("CM");
}
   
