#include <MsTimer2.h>

// Switch on LED on pin 13 each second

int pinBeep = A3;
void flash() {
  static boolean output = HIGH;
  
  digitalWrite(pinBeep, output);
  output = !output;
}

void setup() {
  pinMode(pinBeep, OUTPUT);

  MsTimer2::set(500, flash); // 500ms period
  MsTimer2::start();
}

void loop() {
}

