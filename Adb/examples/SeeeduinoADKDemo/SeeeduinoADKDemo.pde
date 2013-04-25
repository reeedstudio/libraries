//Seeeduino ADK Demo using Niels Brouwers' MicroBridge library. 
//Connect an LED to D12 and a variable resistor(POT) to A0

#include <SPI.h>
#include <Adb.h>

// Adb connection.
Connection * connection;

// Elapsed time for ADC sampling. The rate at which ADC value is sent to Android device.
long lastTime;

//State of LED. Initially OFF. 
uint8_t LEDState=0;

// Event handler for the shell connection. 
// This event handler is called whenever data is sent from Android Device to Seeeduino ADK.
// Any data / command to be sent to I/O of ADK has to be handled here.
//
// For eg: 1.Controlling an ouput port 2.Interacting with a device connected
// to ADK via IIC or Serial Port.

void adbEventHandler(Connection * connection, adb_eventType event, uint16_t length, uint8_t * data)
{
  
  // In this example Data packets contain one byte and it decides the state of a LED connected to D12
  // The size of data is predetermined for this application. Android device also uses the same size.

  if (event == ADB_CONNECTION_RECEIVE)
  {
     if(LEDState != data[0])
     {
         digitalWrite(12, data[0]);   // Change the state of LED 
         Serial.println(data[0],DEC); 
       	 LEDState = data[0];          // Store the State of LED
     }	
  }

}

void setup()
{
  //Serial port debug purpose
  Serial.begin(57600);
  
  // Note start time
  lastTime = millis();

  // Set Digital pin 12 (LED is connected) as output
  pinMode(12,OUTPUT);

  // Initialise the ADB subsystem.  
  ADB::init();

  // Open an ADB stream to the phone's shell. Auto-reconnect. Use any unused port number eg:4568
  connection = ADB::addConnection("tcp:4568", true, adbEventHandler);  

}

void loop()
{
  //Check if ADC needs to be sampled.
  if ((millis() - lastTime) > 20)
  {
    //Read ADC value
    uint16_t data = analogRead(A0);

    //Send the ADC value to Android device as two bytes of data.
    connection->write(2,(uint8_t*)&data);
    lastTime = millis();
  }

  // Poll the ADB subsystem.
  ADB::poll();
}

