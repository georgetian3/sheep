#ifndef SOUND_H
#define SOUND_H

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
//#include <xaudio2.h>

const char* sound_files[] = {
    "../res/welcome.wav",
    "../res/game.wav",
    "../res/clicked.wav",
    "../res/match.wav"
};


void play_sound(int sound) {
    if (sound == -1) {
        return;
    }
    PlaySound(sound_files[sound], NULL, SND_ASYNC | SND_FILENAME | SND_LOOP | SND_NOSTOP);
}






#endif