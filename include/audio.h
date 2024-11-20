#ifndef AUDIO_H
#define AUDIO_H
#include <AL/alut.h>

enum SoundType {

    SOUND_TEST = 0


};

typedef struct Sound {
    ALuint buffer;
    ALuint source;
    ALint status;

} Sound;



void load_sounds(Sound sounds[10]);
void stop_sound(Sound* sound);
void loop_sound(Sound* sound);
void play_sound (Sound* sound);

#endif