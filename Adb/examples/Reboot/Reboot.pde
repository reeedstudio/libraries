#include <SPI.h>
#include <Adb.h>

// Adb connection.
Connection * shell;

void setup()
{

  // Initialise serial port
  Serial.begin(57600);
  
  // Initialise the ADB subsystem.  
  ADB::init();

  // Open an ADB stream to the phone's shell. Auto-reconnect
  shell = ADB::addConnection("shell:", true, NULL);  
}

void loop()
{
  
  // DFRobot romeo buttons are connected to analog input pin 0.
  if (analogRead(A7) == 0 && shell->isOpen())
    shell->writeString("reboot\n");

  // Poll the ADB subsystem.
  ADB::poll();
}

