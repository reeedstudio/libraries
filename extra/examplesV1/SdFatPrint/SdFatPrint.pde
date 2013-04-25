/*
 * Print Example
 *
 * This sketch shows how to use the Arduino Print class with SdFat.
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
  
  // create a new file
  char name[] = "PRINT00.TXT";
  for (uint8_t i = 0; i < 100; i++) {
    name[5] = i/10 + '0';
    name[6] = i%10 + '0';
    // only create new file for write
    if (file.open(name, O_CREAT | O_EXCL | O_WRITE)) break;
  }
  if (!file.isOpen()) error ("file.create");
  PgmPrintln("Printing to: ");
  Serial.println(name);
  
  // print 100 line to file
  for (uint8_t i = 0; i < 100; i++) {
    file.print("line ");
    file.print(i, DEC);
    file.print(" millis = ");
    file.println(millis());
  }
  // force write of all data to the SD card
  if (file.writeError || !file.sync()) error ("print or sync");
  PgmPrintln("Done");
}
void loop(void){}
