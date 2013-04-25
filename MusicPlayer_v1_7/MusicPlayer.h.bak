//  File MusicPlayer.h
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
#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "pins_config.h"
#include "vs10xx.h"
#include "newSDLib.h"
#define READ_BUF_LEN  32
#define VOLUME_UP   0
#define VOLUME_DOWN 1
#define CHANNEL_LEFT  0
#define CHANNEL_RIGHT 1

#define MAXVOL 0
#define SILENT 0xfe

#define MODE_NORMAL      0
#define MODE_SHUFFLE     1
#define MODE_REPEAT_LIST 2
#define MODE_REPEAT_ONE  3

#define MAX_SONG_TOTAL_NUM 100

/******************************************************************************/
#define ENABLE_NUM_DIGIT 8 //for pin[7..0]
#define ENABLE_NUM_ANALOG 2//for A4/A5

/** Playing states. \todo safe rewind */
volatile typedef enum {
  PS_PLAY = 0,      /**< Continue playing, do nothing special                   */
  PS_PAUSE,         /**< Pause play                                             */
  PS_STOP,          /**< Stop play                                              */
  PS_VOLUME_UP,     /**< Request volume up                                      */
  PS_VOLUME_DOWN,   /**< Request volume down                                    */
  PS_NEXT_SONG,     /**< Request next song                                      */
  PS_PREVIOUS_SONG, /**< Request previous song                                  */
  PS_FAST_FORWAD,   /**< Request to start fast forward                          */
  PS_FAST_REWIND,   /**< Request to start fast rewind                           */
  PS_NORMAL_PLAY,   /**< Request to play according to the order of the list     */
  PS_SHUFFLE_PLAY,  /**< Request to shuffle play according to the list          */
  PS_REPEAT_LIST,   /**< Request to repeat play according to the list           */
  PS_REPEAT_ONE,    /**< Request to repeat play a song according to in the list */
  PS_RECORDING      /**< Recording mode */
} playingstatetype;
extern  playingstatetype playingState;
/****************structure for songs playlist*****************/
typedef struct songsPlaylist
{
  unsigned int songIndex[MAX_SONG_TOTAL_NUM];
  unsigned char songTotalNum;//total number of songs in the playlist
  unsigned char currentSongNum;
}spl_t;
/*****************class for the music player*******************/
class MusicPlayer
{
  public:
    void begin();
    void playList();
    void playSong(char *songName);
    void playSong(unsigned int songIndex);
    void setVolume(unsigned char volume) {vs1053.setVolume(volume, volume); _volume = volume;}
    void adjustVolume(boolean UpOrDown, unsigned char NumSteps = 6);
 
    void setPlayMode(unsigned char playmode){_playmode = playmode;}
    void creatPlaylist();  
    void addToPlaylist(unsigned int = 0xffff);//add a song to current playlist
    boolean addToPlaylist(char *songName);
    void addToNewPlaylist();//add the current song to a new playlist
    boolean deleteSong(unsigned char songNum);//delete the index of a song in the playlist
    void attachDigitOperation(int pinNum, void (*userFunc)(void), int mode); 
    void attachAnalogOperation(int pinNum, void (*userFunc)(void)); 
    
    void keyEnable()    {Key_Disable = 0;}
    void keyDisable()   {Key_Disable = 1;}
    void analogControlEnable() {Analog_Enable = 1;}
    void digitalControlEnable(){Digital_Enable = 1;}
    void opPlay()  {playingState = PS_PLAY;}
    void opPause() {playingState = PS_PAUSE;}
    void opStop()  {playingState = PS_STOP;}
    void opVolumeUp()    {playingState = PS_VOLUME_UP;}
    void opVolumeDown()  {playingState = PS_VOLUME_DOWN;}
    void opNextSong()    {playingState = PS_NEXT_SONG;}
    void opPreviousSong(){playingState = PS_PREVIOUS_SONG;}
    void opFastForward() {playingState = PS_FAST_FORWAD;}
    void opFastRewind()  {playingState = PS_FAST_REWIND;}
    void opNormalPlay()  {_playmode = MODE_NORMAL;}
    void opShufflePlay() {_playmode = MODE_SHUFFLE;}
    void opRepeatList()  {_playmode = MODE_REPEAT_LIST;}
    void opRepeatOne()   {_playmode = MODE_REPEAT_ONE;}
    
  private:
    unsigned char _volume;
    unsigned int  _currentSongIndex;
    volatile unsigned char _playmode;
    boolean playOrPause;
    boolean Flag_toPlay;
    boolean Key_Disable;
    boolean Analog_Enable;
    boolean Digital_Enable;
    spl_t spl;
    
    void AvailableProcessorTime();
    boolean playlistIsEmpty();
    void playlistInit();
    void scanKey();
    void scanAnalogSensor();
    void scanDigitalSensor();
    void controlLED();
    boolean playlistIsFull();
};

#endif
