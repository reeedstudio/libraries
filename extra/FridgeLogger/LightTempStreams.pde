#include <SdFat.h>
#include <Wire.h>
#include "RTClib.h"

// A simple data logger for the Arduino analog pins

// how many milliseconds between grabbing data and logging it. 1000 ms is once a second
#define LOG_INTERVAL  1000 // mills between entries (reduce to take more/faster data)

// how many milliseconds before writing the logged data permanently to disk
// set it to the LOG_INTERVAL to write each time (safest)
// set it to 10*LOG_INTERVAL to write all data every 10 datareads, you could lose up to 
// the last 10 reads if power is lost but it uses less power and is much faster!
#define SYNC_INTERVAL 1000 // mills between calls to flush() - to write data to the card
uint32_t syncTime = 0; // time of last sync()

#define ECHO_TO_SERIAL   1 // echo data to serial port
#define WAIT_TO_START    0 // Wait for serial input in setup()

// the digital pins that connect to the LEDs
#define redLEDpin 2
#define greenLEDpin 3

// The analog pins that connect to the sensors
#define photocellPin 0       // analog 0
#define tempPin 1            // analog 1
#define BANDGAPREF 14        // special indicator that we want to measure the bandgap
#define aref_voltage 3.3     // we tie 3.3V to ARef and measure it with a multimeter!
#define bandgap_voltage 1.1  // this is not super guaranteed but its not -too- off
//------------------------------------------------------------------------------
// for the data logging shield, we use digital pin 10 for the sd cs line
const int chipSelect = 10;

// Serial Stream
ArduinoOutStream cout(Serial);

// file system
SdFat sd;

// the logging file
ofstream logfile;

RTC_DS1307 RTC;  // define the Real Time Clock object
//------------------------------------------------------------------------------
// operator that formats date/time
ostream& operator << (ostream& os, DateTime& dt) {
  os << dt.year() << '/' << int(dt.month()) << '/' << int(dt.day()) << ',';
  os << int(dt.hour()) << ':' << setfill('0') << setw(2) << int(dt.minute());
  os << ':' << setw(2) << int(dt.second()) << setfill(' ');
  return os;
}
//------------------------------------------------------------------------------
// call back for file timestamps
void dateTime(uint16_t* date, uint16_t* time) {
    DateTime now = RTC.now();

  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(now.year(), now.month(), now.day());

  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(now.hour(), now.minute(), now.second());
}
//------------------------------------------------------------------------------
void error(char *str) {
  // red LED indicates error
  digitalWrite(redLEDpin, HIGH);
  sd.errorHalt(str);
}
//------------------------------------------------------------------------------
void setup(void) {
  Serial.begin(9600);
  
  // use debugging LEDs
  pinMode(redLEDpin, OUTPUT);
  pinMode(greenLEDpin, OUTPUT);
  
#if WAIT_TO_START
  cout << "Type any character to start" << endl;
  while (!Serial.available());
#endif //WAIT_TO_START

  // connect to RTC
  Wire.begin();
  if (!RTC.begin()) error("RTC failed");

  // set date time callback function
  SdFile::dateTimeCallback(dateTime);

  // see if the card is present and can be initialized:
  if (!sd.init(SPI_HALF_SPEED, chipSelect)) sd.initErrorHalt();
  
  // create a new file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (!sd.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile.open(filename);
      break;  // leave the loop!
    }
  }
  
  if (!logfile.is_open()) error("couldnt create file");
  cout <<"Logging to: " << filename << endl;

  // file header
  logfile << "millis,stamp,date,time,light,temp,vcc" << endl; 
#if ECHO_TO_SERIAL
  cout << "millis,stamp,date,time,light,temp,vcc" << endl;
#endif //ECHO_TO_SERIAL
 
  // If you want to set the aref to something other than 5v
  analogReference(EXTERNAL);
}
//------------------------------------------------------------------------------
void loop(void) {
  // milliseconds since starting
  uint32_t m = millis();
  
  // wait for time to be a multiple of logging interval
  do {
    m = millis();
  } while (m % LOG_INTERVAL);
  
  digitalWrite(greenLEDpin, HIGH);
  
  // fetch the time
  DateTime now = RTC.now();
  
  // read the photo cell
  analogRead(photocellPin);
  delay(10); 
  int photocellReading = analogRead(photocellPin);  
  
  // read the temperature
  analogRead(tempPin); 
  delay(10);
  int tempReading = analogRead(tempPin);    
  
  // converting that reading to voltage, for 3.3v arduino use 3.3, for 5.0, use 5.0
  float voltage = tempReading * aref_voltage / 1024;  
  float temperatureC = (voltage - 0.5) * 100 ;
  float temperatureF = (temperatureC * 9 / 5) + 32;
  
  // Get the estimated 'VCC' voltage by measuring the internal 1.1v ref
  analogRead(BANDGAPREF); 
  delay(10);
  int refReading = analogRead(BANDGAPREF); 
  float supplyvoltage = (bandgap_voltage * 1024) / refReading; 
  
  // log the data
  logfile << m << ',' << now.unixtime() << ',' << now;
  logfile << ',' << photocellReading << ',' << temperatureF;
  logfile << ',' << supplyvoltage << endl;
#if ECHO_TO_SERIAL
  cout << m << ',' << now.unixtime() << ',' << now;
  cout << ',' << photocellReading << ',' << temperatureF;
  cout << ',' << supplyvoltage << endl;
#endif // ECHO_TO_SERIAL

  digitalWrite(greenLEDpin, LOW);

  // Now we write data to disk! Don't sync too often
  // requires 2048 bytes of I/O to sd card
  // which uses a bunch of power and takes time
  if ((millis() - syncTime) >= SYNC_INTERVAL) {
    syncTime = millis();
  
    // blink LED to show we are syncing data to the card & updating FAT!
    digitalWrite(redLEDpin, HIGH);
    logfile.flush();
    digitalWrite(redLEDpin, LOW);
  }
  // check for errors
  if (!logfile.good()) error("write error");
}
