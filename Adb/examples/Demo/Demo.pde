#include <SPI.h>
#include <Adb.h>
#include <Servo.h>

// Hardware servos
Servo servos[2];

// Adb connection.
Connection * connection;

// Elapsed time for ADC sampling
long lastTime;

// Event handler for the shell connection. 
void adbEventHandler(Connection * connection, adb_eventType event, uint16_t length, uint8_t * data)
{
  int i;

  // Data packets contain two bytes, one for each servo, in the range of [0..180]
  if (event == ADB_CONNECTION_RECEIVE)
  {
    servos[0].write(data[0]);
    servos[1].write(data[1]);
  }

}

void setup()
{

  // Initialise serial port
  Serial.begin(57600);
  
  // Note start time
  lastTime = millis();
  
  // Attach servos
  servos[0].attach(2);
  servos[1].attach(3);

  // Initialise the ADB subsystem.  
  ADB::init();

  // Open an ADB stream to the phone's shell. Auto-reconnect
  connection = ADB::addConnection("tcp:4567", true, adbEventHandler);  
}

void loop()
{
  
  if ((millis() - lastTime) > 20)
  {
    uint16_t data = analogRead(A0);
    connection->write(2, (uint8_t*)&data);
    lastTime = millis();
  }

  // Poll the ADB subsystem.
  ADB::poll();
}

