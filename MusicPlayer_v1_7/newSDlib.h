//  File nesSDlib.h
//
//  For more details about the product please check http://www.seeedstudio.com/depot/
//  Copyright (c) 2012 seeed technology inc.
//  Author: Frankie.Chu
//  Version: 2.0
//  Time: June 3, 2012
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
#ifndef NEWSDLIB_H
#define NEWSDLIB_H

#include <Fat16.h>
#include <Fat16Util.h>

extern SdCard card;
extern Fat16 file;

void error_P(const char* str) ;
// store error strings in flash to save RAM
#define error(s) error_P(PSTR(s))
class newSD
{
  public:
    unsigned char init();
    unsigned char openFile(char *fileName);
    unsigned char openFile(unsigned int index);
    int  readFile(byte *buffer,unsigned int len);
    //unsigned char traverseDir(const char* fileNameExt);
    boolean traverseRootSong(unsigned int songIndex[], unsigned char &totalNum);//traverse the root directory to find all the songs and save the indexes of them
    boolean findIndex(char *fileName,unsigned int &songindex);
  private:
    void ucharArray2String(const unsigned char songName[]);
};
extern newSD newsd;

#endif

