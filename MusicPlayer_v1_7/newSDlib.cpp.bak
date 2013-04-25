//  File newSDlib.cpp
//
//  For more details about the product please check http://www.seeedstudio.com/depot/
//  Copyright (c) 2012 seeed technology inc.
//  Author: Frankie.Chu
//  Version: 2.0
//  Time: June 4, 2012
//  Changing records:
//    
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#include <Fat16.h>
#include <Fat16Util.h>
#include "newSDLib.h"
#include "pins_config.h"

SdCard card;
Fat16 file;

newSD newsd;
// format 8.3 name for directory entry
static uint8_t make83Name(const char* str, uint8_t* name) {
  uint8_t c;
  uint8_t n = 7;  // max index for part before dot
  uint8_t i = 0;
  // blank fill name and extension
  while (i < 11) name[i++] = ' ';
  i = 0;
  while ((c = *str++) != '\0') {
    if (c == '.') {
      if (n == 10) return false;  // only one dot allowed
      n = 10;  // max index for full 8.3 name
      i = 8;   // place for extension
    } else {
      // illegal FAT characters
      PGM_P p = PSTR("|<>^+=?/[];,*\"\\");
      uint8_t b;
      while ((b = pgm_read_byte(p++))) if (b == c) return false;
      // check length and only allow ASCII printable characters
      if (i > n || c < 0X21 || c > 0X7E) return false;
      // only upper case allowed in 8.3 names - convert lower to upper
      name[i++] = c < 'a' || c > 'z' ?  c : c + ('A' - 'a');
    }
  }
  // must have a file name, extension is optional
  return name[0] != ' ';
}
/***************************************/
void error_P(const char* str) {
  PgmPrint("error: ");
  SerialPrintln_P(str);
  if (card.errorCode) {
    PgmPrint("SD error: ");
    Serial.print(card.errorCode, HEX);
  }
}

unsigned char newSD::init()
{
  //SPI speed: 0 - F_CPU/2, 1 - F_CPU/4
  if (!card.init(0,SD_CS_PIN))
  {
    error("card.init failed");
    return 0;
  }
  // initialize a FAT16 volume
  if (!Fat16::init(&card))
  {
    error("Fat16::init");
    return 0;
  }
  return 1;
}

unsigned char newSD::openFile(char *fileName)
{
  if (file.open(fileName, O_READ)) {
    Serial.write(fileName);
    Serial.println(" opened");
    return 1;
  }
  else{
    error("open file failed");
    return 0;
  }
}

unsigned char newSD::openFile(unsigned int index)
{
  if (file.open(index, O_READ)) {
   // Serial.print(index);
  //  Serial.println(" opened");
    return 1;
  }
  else{
    error("open file failed");
    return 0;
  }
}

int newSD::readFile(byte *buffer,unsigned int len)
{
  int readLen = 0;
  readLen = file.read(buffer,len);
  return readLen;
}

boolean newSD::traverseRootSong(unsigned int songIndex[], unsigned char &totalNum)
{
  dir_t* p;            // pointer to  directory entry
  unsigned int _rootDirEntryCount = file.rootDirEntryCount();
  Serial.println("All songs in the root directory:");
  for (unsigned int index = 0; index < _rootDirEntryCount; index++) 
  {
    if (!(p = file.dbgCacheDir(index))) return false;
    if (p->name[0] == DIR_NAME_FREE) return true;
    else if(p->name[0] == DIR_NAME_DELETED)continue;
    else if((p->name[8] == 'M')&&(p->name[9] == 'P')&&(p->name[10] == '3')){
      songIndex[totalNum] = index;
      totalNum ++;
      ucharArray2String(p->name);
    }
    else if((p->name[8] == 'W')&&(p->name[9] == 'A')&&(p->name[10] == 'V')){
      songIndex[totalNum] = index;
      totalNum ++;
      ucharArray2String(p->name);
    }
    else if((p->name[8] == 'W')&&(p->name[9] == 'M')&&(p->name[10] == 'A')){
      songIndex[totalNum] = index;
      totalNum ++;
      ucharArray2String(p->name);
    }
    else if((p->name[8] == 'M')&&(p->name[9] == 'I')&&(p->name[10] == 'D')){
      songIndex[totalNum] = index;
      totalNum ++;
      ucharArray2String(p->name);
    }
    else if((p->name[8] == 'O')&&(p->name[9] == 'G')&&(p->name[10] == 'G')){
      songIndex[totalNum] = index;
      totalNum ++;
      ucharArray2String(p->name);
    }
    if(totalNum == 100)return true;
  }
}
/*************************************************/
boolean newSD::findIndex(char *fileName,unsigned int &songindex)
{
  unsigned char dname[11];   // name formated for dir entry
  dir_t* p;            // pointer to cached dir entry
  // error if invalid name
  unsigned int _rootDirEntryCount = file.rootDirEntryCount();
  if (!make83Name(fileName, dname)) return false;//false

  for (uint16_t index = 0; index < _rootDirEntryCount; index++) {
    if (!(p = file.dbgCacheDir(index))) return false;//false
    if (p->name[0] == DIR_NAME_FREE || p->name[0] == DIR_NAME_DELETED) {
      // done if no entries follow
      if (p->name[0] == DIR_NAME_FREE) break;
    } else if (!memcmp(dname, p->name, 11)) {
      songindex = index;
      return true;
    }
  }
}

/****************************************/
void newSD::ucharArray2String(const unsigned char songName[])
{
  String str = "";
  for(unsigned char i = 0;i < 11;i ++)
  {
    if(i == 8)str += '.';
    if(songName[i] != 0x20) str += (char)songName[i];
  }
  Serial.println(str);
}
/***************The end*************************/
