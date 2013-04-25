#include <SPI.h>
#include <Adb.h>

Connection * shell;

// Event handler for the shell connection. 
void adbEventHandler(Connection * connection, adb_eventType event, uint16_t length, uint8_t * data)
{
  int i;

  if (event == ADB_CONNECTION_RECEIVE)
    for (i=0; i<length; i++)
      Serial.print(data[i]);

}

void setup()
{

  // Initialise serial port
  Serial.begin(57600);

  // Initialise the ADB subsystem.  
  ADB::init();

  // Open an ADB stream to the phone's shell. Auto-reconnect
  shell = ADB::addConnection("shell:", true, adbEventHandler);  
}

void loop()
{
  byte ch;
  
  // Check for incoming serial data
  if (Serial.available() > 0)
  {
    // read the incoming byte:
    ch = Serial.read();
    
    // Write to shell
    if (shell->status == ADB_OPEN)
      shell->write(1, &ch);
    else
      Serial.print("Shell not open");

  }

  // Poll the ADB subsystem.
  ADB::poll();
}

