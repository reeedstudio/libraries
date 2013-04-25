/*********************************************/
version:v1.7

1.Default seting:play mode = NORMAL play. MusicPlayer::setPlayMode();
		 keys      = ENBALE	  MusicPlayer::keyDisable();
 scan available digit pins = DISABLE	  MusicPlayer::digitalControlEnable();	
 scan available analog pins= DISABLE 	  MusicPlayer::analogControlEnable();
 creat an empty playlist
 NOTE:ALL THE SONGS TO PLAY SHOULD BE IN THE ROOT DIRECTORY.

2.Three different ways to play music:
  First:As the playlist is empty,you can add some other songs to it.Set the play mode and then play the playlist.
  Second:You can call the function MusicPlayer::creatPlaylist() to add all the songs to the playlist if the it is empty or to add the current song to a new playlist.Set the play mode and then play the playlist.
  Third:play a song with name and it will add to the playlist,too.

10:45 2012/6/19 by Frankie.Chu at Seeed Studio.
/**********************************************/