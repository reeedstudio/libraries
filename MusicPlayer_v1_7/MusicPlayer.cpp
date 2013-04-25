//  File MusicPlayer.cpp
//
//  For more details about the product please check http://www.seeedstudio.com/depot/
//  Copyright (c) 2012 seeed technology inc.
//  Author: Frankie.Chu
//  Version: 2.0
//  Time: June 10, 2012
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
#include "pins_config.h"
#include "vs10xx.h"
#include "newSDLib.h"
#include "MusicPlayer.h"

typedef void (*voidFunctionPtr)(void);
volatile static voidFunctionPtr digitCtrlFunc[ENABLE_NUM_DIGIT];
char _DigitPinLeval[ENABLE_NUM_DIGIT];
char _DigitPinNum[ENABLE_NUM_DIGIT];

volatile static voidFunctionPtr analogCtrlFunc[ENABLE_NUM_ANALOG];
char _AnalogPinNum[ENABLE_NUM_DIGIT];
/******************** Global enum variable ********************/
playingstatetype playingState;

/**************************************************************/
void MusicPlayer::begin()
{
  initIOForLED();
  if(!Key_Disable)initIOForKeys();
  vs1053.init();
  newsd.init();
  playlistInit();
  setVolume(40);
  _playmode = MODE_NORMAL;
}

/**************************************************************/
void MusicPlayer::playList()
{
  if(!playlistIsEmpty())
  {
    while(1)
    {
      playOrPause = 1;
      playingState = PS_PLAY;
      unsigned int songindex = spl.songIndex[spl.currentSongNum];
      playSong(songindex);
      switch(playingState)
      {
        case PS_STOP:         playOrPause = 0;GREEN_LED_OFF();return;
        case PS_NEXT_SONG:    if(_playmode == MODE_SHUFFLE) spl.currentSongNum = random(0,spl.songTotalNum);
                              else if(spl.currentSongNum == spl.songTotalNum - 1)   spl.currentSongNum = 0;
                              else ++ spl.currentSongNum;
                              playOrPause = 1;
                              break;
        case PS_PREVIOUS_SONG:if(_playmode == MODE_SHUFFLE) spl.currentSongNum = random(0,spl.songTotalNum);
                              else if(spl.currentSongNum == 0)  spl.currentSongNum = spl.songTotalNum - 1;
                              else -- spl.currentSongNum;
                              playOrPause = 1;
                              break;
        default:              if(_playmode == MODE_SHUFFLE) spl.currentSongNum = random(0,spl.songTotalNum);
                              else if(_playmode == MODE_NORMAL)
                              {
                                if(spl.currentSongNum == spl.songTotalNum - 1)
                                {
                                  playingState = PS_STOP;
                                  return;
                                }
                                else spl.currentSongNum ++;
                              }
                              else if(_playmode == MODE_REPEAT_LIST)
                              {
                                if(spl.currentSongNum == spl.songTotalNum - 1)   spl.currentSongNum = 0;
                                else spl.currentSongNum++;
                              }
                              break;
      }
    }
  }
}
/**************************************************************/
void MusicPlayer::playSong(char *songName)
{
  Flag_toPlay = 1;
  if(addToPlaylist(songName))
  {
    playList();
  }
}
/**************************************************************/
void MusicPlayer::playSong(unsigned int songIndex)
{
  vs1053.softReset();
  if(!newsd.openFile(songIndex)){//open music file
    return;
  }
  Serial.print("Song ");
  Serial.print(spl.currentSongNum);
  Serial.println(" opened");
  _currentSongIndex = songIndex;
  int readLen = 0;
  byte readBuf[READ_BUF_LEN];
  while(1){
    readLen = newsd.readFile(readBuf,READ_BUF_LEN);  
    vs1053.writeData(readBuf,readLen);
    if(readLen < READ_BUF_LEN)
    {
      vs1053.writeRegister(SPI_MODE,0,SM_OUTOFWAV);
      vs1053.sendZerosToVS10xx();
      break;
    }
    while(!vs1053.readDREQ())
    {
      while(1){
        AvailableProcessorTime();
        if((playingState == PS_STOP)||(playingState == PS_NEXT_SONG)||(playingState == PS_PREVIOUS_SONG))
        {
          if(file.close() == 0)//close file
             error("close file failed");
          return;
        }
        else 
        {
          switch(playingState)
          {
            case PS_PLAY:       playOrPause = 1;break;
            case PS_PAUSE:      playOrPause = 0;break;
            case PS_VOLUME_UP:  adjustVolume(VOLUME_UP);break;
            case PS_VOLUME_DOWN:adjustVolume(VOLUME_DOWN);break;
            default:break;
          }
        }
        if(playOrPause == 1)break;
      }
    }
  }
  Serial.println("played over\r\n");
  if(file.close() == 0)//close file
  {
    error("close file failed");
  }
}


/**************************************************************/
void MusicPlayer::adjustVolume(boolean UpOrDown, unsigned char NumSteps)
{
  if(UpOrDown == VOLUME_UP)
  {
    if(_volume < NumSteps)_volume = MAXVOL;
    else _volume -= NumSteps;
  }
  else
  {
    if(_volume > SILENT - NumSteps)_volume = SILENT;
    else _volume += NumSteps;
  }
  setVolume(_volume);
  if(playOrPause) playingState = PS_PLAY;
  else            playingState = PS_PAUSE;
}

/**************************************************************/
void MusicPlayer::creatPlaylist()
{
  if(playlistIsEmpty())
  {
    if(newsd.traverseRootSong(spl.songIndex, spl.songTotalNum))
    {
      for(unsigned char i = 0;i < spl.songTotalNum;i ++)
      {
        if(_currentSongIndex == spl.songIndex[i]) spl.currentSongNum = i;
        else spl.currentSongNum = 0;
      }
    }
  }
  else
  {
    addToNewPlaylist();
  }
}

/**************************************************************/
void MusicPlayer::addToPlaylist(unsigned int songIndex)//add a song to current playlist
{
  if(songIndex == 0xffff)songIndex = _currentSongIndex;
  for(unsigned char i = 0;i < spl.songTotalNum;i ++)
  {
    if(songIndex == spl.songIndex[i])
    {
      if(Flag_toPlay){
        spl.currentSongNum = i;
        Flag_toPlay = 0;
      }
      Serial.println("The song is exist.");
      return;
    }
  }
  if(playlistIsFull()) deleteSong(0);
  spl.songIndex[spl.songTotalNum] = songIndex;
  ++ spl.songTotalNum;
  if(Flag_toPlay){
    spl.currentSongNum = spl.songTotalNum - 1;
    Flag_toPlay = 0;
  }
}
/**************************************************************/
boolean MusicPlayer::addToPlaylist(char *songName)//add a song to current playlist
{
  unsigned int songindex;
  if(newsd.findIndex(songName,songindex))
  {
    addToPlaylist(songindex);
    return true;
  }
  else return false;
}

/**************************************************************/
void MusicPlayer::addToNewPlaylist()
{
  playlistInit();//clear the current playlist
  addToPlaylist();
}

/**************************************************************/
boolean MusicPlayer::deleteSong(unsigned char songNum)
{
  if(songNum > spl.songTotalNum - 1)return false;
  for(unsigned char i = songNum + 1;i < spl.songTotalNum;i ++)
  {
    spl.songIndex[i - 1] = spl.songIndex[i];
  }
  --spl.songTotalNum;
  if(songNum == spl.currentSongNum)
  {
    playingState = PS_STOP;
  }
  return true;
}

/**************************************************************/
void MusicPlayer::attachDigitOperation(int pinNum, void (*userFunc)(void), int mode)
{
  pinMode(pinNum,INPUT);
  _DigitPinLeval[pinNum] = mode;
  _DigitPinNum[pinNum]   = pinNum;
  digitCtrlFunc[pinNum]  = userFunc;
}
/**************************************************************/
void MusicPlayer::attachAnalogOperation(int pinNum, void (*userFunc)(void))
{
  pinMode(pinNum,INPUT);
  _AnalogPinNum[pinNum - A4]  = pinNum;
  analogCtrlFunc[pinNum - A4] = userFunc;
}
/**************************************************************/
void MusicPlayer::AvailableProcessorTime()
{
  if(!Key_Disable)scanKey();
  if(Analog_Enable)scanAnalogSensor();
  if(Digital_Enable)scanDigitalSensor();
  controlLED();
}
/**************************************************************/
boolean MusicPlayer::playlistIsEmpty()
{
  if(spl.songTotalNum == 0)return true;
  else return false;
}
void MusicPlayer::playlistInit()
{
  unsigned char i;
  for(i = 0;i < MAX_SONG_TOTAL_NUM;i ++)
  {
    spl.songIndex[i] = 0;
  }
  spl.songTotalNum = 0;
  spl.currentSongNum = 0;
}
/**************************************************************/
void MusicPlayer::scanKey()
{
  static unsigned int vu_cnt = 1000;//volume up interval
  static unsigned int vd_cnt = 1000;//volume down interval
  if(digitalRead(KEY_PS) == LOW)
  {
  	playOrPause = !playOrPause;
        if(playOrPause) playingState = PS_PLAY;
        else            playingState = PS_PAUSE;
	delay(10);
	while(digitalRead(KEY_PS) == LOW);//Wait for the button to be released
	delay(10);
  }
  else if(digitalRead(KEY_VD) == LOW) 
  {
    if(-- vd_cnt == 0)
    {
      vd_cnt = 1000;
      playingState = PS_VOLUME_DOWN;
    }
  }
  else if(digitalRead(KEY_VU) == LOW)
  {
    if(-- vu_cnt == 0)
    {
      vu_cnt = 1000;
      playingState = PS_VOLUME_UP;
    }
  }
  else if(digitalRead(KEY_BK) == LOW) 
  {
    playingState = PS_PREVIOUS_SONG;
    delay(10);
    while(digitalRead(KEY_BK) == LOW);//Wait for the button to be released
    delay(10);
  }
  else if(digitalRead(KEY_NT) == LOW) 
  {
    playingState = PS_NEXT_SONG;
    delay(10);
    while(digitalRead(KEY_NT) == LOW);//Wait for the button to be released
    delay(10);
  }
}
/********************************************************/
void MusicPlayer::scanAnalogSensor()
{
  for(unsigned char i = 0;i < ENABLE_NUM_ANALOG;i ++)
  {
    if(analogCtrlFunc[i]) analogCtrlFunc[i]();
  }
}
/********************************************************/
void MusicPlayer::scanDigitalSensor()
{
  for(unsigned char i = 0;i < ENABLE_NUM_DIGIT;i ++)
  {
    if(digitCtrlFunc[i])
      if(digitalRead(_DigitPinNum[i]) == _DigitPinLeval[i])
      {
        digitCtrlFunc[i]();
        while(digitalRead(_DigitPinNum[i]) == _DigitPinLeval[i]);
      }
  }
}
/**************************************************************/
void MusicPlayer::controlLED()
{
  byte static flagOn = 1;
  int static count = 5000;
  if(playOrPause == 0)GREEN_LED_ON();
  else
    if(--count == 0)
    {
      count = 5000;
      if(flagOn == 1) 
      {
        GREEN_LED_ON();
        flagOn = 0;
      }
      else
      {
        GREEN_LED_OFF();
        flagOn = 1;
      }
    }
}
boolean MusicPlayer::playlistIsFull()
{
  if(spl.songTotalNum != MAX_SONG_TOTAL_NUM)return false;
  else return true;
}
