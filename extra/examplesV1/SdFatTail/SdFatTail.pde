/*
 * This sketch reads and prints the tail of all files
 * created by SdFatAppend.pde, SdFatPrint.pde, and
 * SdFatWrite.pde.
 */
#include <SdFat.h>

SdFat sd;

SdFile file;

// store error strings in flash to save RAM
#define error(s) sd.errorHalt_P(PSTR(s))

void setup(void) {
  Serial.begin(9600);
  Serial.println();
  Serial.println("type any character to start");
  while (!Serial.available());
  Serial.println();
  
  // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.
  if (!sd.init(SPI_HALF_SPEED)) sd.initErrorHalt();
  
  // start at beginning of root directory
  sd.vwd()->rewind();
}
/*
 * Print tail of all SdFat example files
 */
void loop(void) {
  dir_t dir;
  char name[13];
  
  // read next directory entry in current working directory
  if (sd.vwd()->readDir(&dir) != sizeof(dir)) {
    Serial.println("End of Directory");
    while(1);
  }
  
  // check for file name "APPEND.TXT", "PRINT*.TXT"
  // or "WRITE*.TXT"
  // first 8 bytes are blank filled name
  // last three bytes are blank filled extension
  if ((strncmp((char *)dir.name, "APPEND  ", 8) &&
      strncmp((char *)dir.name, "PRINT", 5) &&
      strncmp((char *)dir.name, "WRITE", 5)) ||
      strncmp((char *)&dir.name[8], "TXT", 3)) {
        return;
  }
  // format file name
  SdFile::dirName(dir, name);
  
  // remember position in directory
  uint32_t pos = sd.vwd()->curPosition();
  
  // open file
  if (!file.open(name, O_READ)) error("file.open failed");
  
  // restore root position
  if (!sd.vwd()->seekSet(pos)) error("seekSet failed");
  
  // print file name message
  Serial.print("Tail of: ");
  Serial.println(name);
  
  // position to tail of file
  if (file.fileSize() > 100) {
    if (!file.seekSet(file.fileSize() - 100)) error("file.seekSet failed");
  }
  int16_t c;
  // find end of line  
  while ((c = file.read()) > 0 && c != '\n');
  
  // print rest of file
  while ((c = file.read()) > 0) Serial.write((char)c);
  file.close();
  Serial.println();
}
