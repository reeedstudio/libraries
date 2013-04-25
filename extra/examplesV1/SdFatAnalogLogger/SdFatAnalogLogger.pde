// A simple data logger for the Arduino analog pins
#define LOG_INTERVAL  1000 // mills between entries
#define SENSOR_COUNT     3 // number of analog pins to log
#define ECHO_TO_SERIAL   1 // echo data to serial port
#define WAIT_TO_START    1 // Wait for serial input in setup()
#define SYNC_INTERVAL 1000 // mills between calls to sync()
uint32_t syncTime = 0;     // time of last sync()

#include <SdFat.h>

// file system
SdFat sd;

// log file
SdFile file;

// store error strings in flash to save RAM
#define error(s) sd.errorHalt_P(PSTR(s))
//------------------------------------------------------------------------------
void setup(void) {
  Serial.begin(9600);
  Serial.println();
  
#if WAIT_TO_START
  Serial.println("Type any character to start");
  while (!Serial.available());
#endif //WAIT_TO_START

  // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.
  if (!sd.init(SPI_HALF_SPEED)) sd.initErrorHalt();
    
  // create a new file
  char name[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    name[6] = i/10 + '0';
    name[7] = i%10 + '0';
    if (file.open(name, O_CREAT | O_EXCL | O_WRITE)) break;
  }
  if (!file.isOpen()) error ("file.create");
  Serial.print("Logging to: ");
  Serial.println(name);

  // write header
  file.writeError = 0;
  file.print("millis");
#if ECHO_TO_SERIAL 
  Serial.print("millis");
#endif //ECHO_TO_SERIAL

#if SENSOR_COUNT > 6
#error SENSOR_COUNT too large
#endif //SENSOR_COUNT

  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
    file.print(",sens");file.print(i, DEC);    
#if ECHO_TO_SERIAL
    Serial.print(",sens");Serial.print(i, DEC);
#endif //ECHO_TO_SERIAL
  }
  file.println();  
#if ECHO_TO_SERIAL
  Serial.println();
#endif  //ECHO_TO_SERIAL

  if (file.writeError || !file.sync()) {
    error("write header failed");
  }
}
//------------------------------------------------------------------------------
void loop(void) {
  uint32_t m;

  // wait for time to be a multiple of interval
  do {
    m = millis();
  } while (m % LOG_INTERVAL);
  
  // log time
  file.print(m);  
#if ECHO_TO_SERIAL
  Serial.print(m);
#endif //ECHO_TO_SERIAL
      
  // add sensor data 
  for (uint8_t ia = 0; ia < SENSOR_COUNT; ia++) {
    uint16_t data = analogRead(ia);
    file.write(',');
    file.print(data);
#if ECHO_TO_SERIAL
    Serial.write(',');
    Serial.print(data);
#endif //ECHO_TO_SERIAL
  }
  file.println();  
#if ECHO_TO_SERIAL
  Serial.println();
#endif //ECHO_TO_SERIAL

  if (file.writeError) error("write data failed");
  
  //don't sync too often - requires 2048 bytes of I/O to SD card
  if ((millis() - syncTime) >= SYNC_INTERVAL) {
    syncTime = millis();
    if (!file.sync()) error("sync failed");
  }
  // don't log two samples in same millisecond
  if (m == millis()) delay(1);
}
