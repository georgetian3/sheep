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

typedef enum {None = -1, Welcome, Game, Click, Match} SoundType;

void play_sound(SoundType sound, BOOL loop, BOOL interrupt) {
    if (interrupt) {
        PlaySound(NULL, NULL, 0);
    }
    if (sound == None) {
        return;
    }
    PlaySound(sound_files[sound], NULL, SND_ASYNC | SND_FILENAME | SND_LOOP * loop | SND_NOSTOP);
}






#endif