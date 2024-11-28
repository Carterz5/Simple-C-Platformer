#include "audio.h"





void play_sound (Sound* sound){

    alSourcePlay(sound->source);

}

void stop_sound(Sound* sound){


    alSourceStop(sound->source);



}

void load_sounds(Sound sounds[3]){

    sounds[SOUND_JUMP].buffer = alutCreateBufferFromFile("../assets/audio/jump.wav");
    alGenSources(1, &sounds[SOUND_JUMP].source);
    alSourcei(sounds[SOUND_JUMP].source, AL_BUFFER, sounds[SOUND_JUMP].buffer);
    alSourcef(sounds[SOUND_JUMP].source, AL_GAIN, 0.2);

    sounds[SOUND_DEATH].buffer = alutCreateBufferFromFile("../assets/audio/death_jason.wav");
    alGenSources(1, &sounds[SOUND_DEATH].source);
    alSourcei(sounds[SOUND_DEATH].source, AL_BUFFER, sounds[SOUND_DEATH].buffer);
    alSourcef(sounds[SOUND_DEATH].source, AL_GAIN, 0.2);



    sounds[SOUND_MUSIC].buffer = alutCreateBufferFromFile("../assets/audio/platformer_song.wav");
    alGenSources(1, &sounds[SOUND_MUSIC].source);
    alSourcei(sounds[SOUND_MUSIC].source, AL_BUFFER, sounds[SOUND_MUSIC].buffer);
    alSourcei(sounds[SOUND_MUSIC].source, AL_LOOPING, 1);
    alSourcef(sounds[SOUND_MUSIC].source, AL_GAIN, 0.2);



    

}