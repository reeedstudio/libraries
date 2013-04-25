/*
 * Remove Example
 *
 * This sketch shows how to use remove() to delete
 * the file created by the SdFatAppend.pde example.
 */
#include <SdFat.h>
#include <SdFatUtil.h> // use functions to print strings from flash memory

SdFat sd;

SdFile file;

// store error strings in flash to save RAM
#define error(s) sd.errorHalt_P(PSTR(s))

void setup(void) {
  Serial.begin(9600);
  Serial.println();
  PgmPrintln("Type any character to start");
  while (!Serial.available());
  
  // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.
  if (!sd.init(SPI_HALF_SPEED)) sd.initErrorHalt();

  char name[] = "APPEND.TXT";
  if (!file.open(name, O_WRITE)) {
    PgmPrint("Can't open "); 
    Serial.println(name);
    PgmPrintln("Run the append example to create the file.");
    error("file.open failed");
  }
  if (!file.remove()) error("file.remove failed");
  Serial.print(name);
  PgmPrintln(" deleted.");
}

void loop(void) {}
