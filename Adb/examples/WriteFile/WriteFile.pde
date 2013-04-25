#include <SPI.h>
#include <Adb.h>

void setup()
{

  // Initialise serial port
  Serial.begin(57600);

  // Initialise the ADB subsystem.  
  ADB::init();

  // Execute the UNIX shell command 'echo "hello world" > /sdcard/hello', which creates a
  // new text file on the sd card called 'hello', containing the text 'hello world'.
  ADB::addConnection("shell:echo \"hello world\" > /sdcard/hello", false, NULL);
}

void loop()
{
  // Poll the ADB subsystem.
  ADB::poll();
}

