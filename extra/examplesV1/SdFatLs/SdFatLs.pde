/*
 * This sketch will list all files in the root directory and 
 * then do a recursive list of all directories on the SD card.
 *
 */
#include <SdFat.h>
#include <SdFatUtil.h>

SdFat sd;

// store error strings in flash to save RAM
#define error(s) sd.errorHalt_P(PSTR(s))


void setup() {
  Serial.begin(9600);
  PgmPrintln("Type any character to start");
  while (!Serial.available());
  
  PgmPrint("Free RAM: ");
  Serial.println(FreeRam());  
  
  // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.
  if (!sd.init(SPI_HALF_SPEED)) sd.initErrorHalt();
  
  PgmPrint("Volume is FAT");
  Serial.println(sd.vol()->fatType(), DEC);
  Serial.println();

  // list file in root with date and size
  PgmPrintln("Files found in root:");
  sd.ls(LS_DATE | LS_SIZE);
  Serial.println();
  
  // Recursive list of all directories
  PgmPrintln("Files found in all dirs:");
  sd.ls(LS_R);
  
  Serial.println();
  PgmPrintln("Done");
}

void loop() { }
