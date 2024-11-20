#include "audio.h"





void play_sound (Sound* sound){



    // //alSourcei(sound->source, AL_BUFFER, sound->buffer);
    // alSourcePlay(sound->source);


    ALuint buffer;
    ALuint source;
    ALint status;

    buffer = alutCreateBufferFromFile("../assets/audio/jump.wav");

    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcePlay(source);



}


void loop_sound(Sound* sound){


    alSourcei(sound->source, AL_BUFFER, sound->buffer);
    alSourcei(sound->source, AL_LOOPING, 1);
    alSourcef(sound->source, AL_GAIN, 0.5);
    alSourcePlay(sound->source);



}

void stop_sound(Sound* sound){


    alSourceStop(sound->source);



}

void load_sounds(Sound sounds[10]){

    sounds[SOUND_TEST].buffer = alutCreateBufferFromFile("../assets/audio/jump.wav");
    alGenSources(1, &sounds[SOUND_TEST].source);
    alSourcei(sounds[SOUND_TEST].source, AL_BUFFER, sounds[SOUND_TEST].buffer);

}