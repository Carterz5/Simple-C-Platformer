#ifndef AUDIO_H
#define AUDIO_H
#include <AL/alut.h>

enum SoundType {

    SOUND_JUMP = 0,
    SOUND_DEATH = 1,
    SOUND_MUSIC = 2

};

typedef struct Sound {
    ALuint buffer;
    ALuint source;
    ALint status;

} Sound;



void load_sounds(Sound sounds[3]);
void stop_sound(Sound* sound);
void play_sound (Sound* sound);

#endif