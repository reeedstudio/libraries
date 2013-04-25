#include <Wire.h>
#include <ServoShield2.h>

ServoShield2 servos = ServoShield2(64);//Address of 127, using 50Hz mode

void setup() {
  Serial.begin(9600); 
  Serial.println("Initializing...");
  servos.start();
  
  for (int servo = 0; servo < 16; servo++)//Initialize all 16 servos
  {
    servos.setbounds(servo, 1000, 2000);  //Set the minimum and maximum pulse duration of the servo
    servos.setposition(servo, 1500);      //Set the initial position of the servo
  }
  Serial.println("Init Done");
}

void sweep()
{
  Serial.println("Sweeping");
  
  for(int pos = 1000; pos < 2000; pos += 20)  //Move the servos from 0 degrees to 180 degrees 
  {                                       
    for (int i = 0; i < 16; i++)         
      servos.setposition(i, pos);         
     delay(1);                             
  } 
  
  for(int pos = 2000; pos >= 1000; pos -= 20) //Move the servos from 180 degrees to 0 degrees 
  {                                
    for (int i = 0; i < 16; i++)          
      servos.setposition(i, pos);        
    delay(1);                            
  } 
}

void loop() {
  sweep();
}