#include "audio.h"





void play_sound (Sound* sound){

    alSourcePlay(sound->source);

}

void stop_sound(Sound* sound){


    alSourceStop(sound->source);



}

void load_sounds(Sound sounds[10]){

    sounds[SOUND_JUMP].buffer = alutCreateBufferFromFile("../assets/audio/jump.wav");
    alGenSources(1, &sounds[SOUND_JUMP].source);
    alSourcei(sounds[SOUND_JUMP].source, AL_BUFFER, sounds[SOUND_JUMP].buffer);

    sounds[SOUND_DEATH].buffer = alutCreateBufferFromFile("../assets/audio/death_jason.wav");
    alGenSources(1, &sounds[SOUND_DEATH].source);
    alSourcei(sounds[SOUND_DEATH].source, AL_BUFFER, sounds[SOUND_DEATH].buffer);



    sounds[SOUND_MUSIC].buffer = alutCreateBufferFromFile("../assets/audio/snake_song_2.wav");
    alGenSources(1, &sounds[SOUND_MUSIC].source);
    alSourcei(sounds[SOUND_MUSIC].source, AL_BUFFER, sounds[SOUND_MUSIC].buffer);
    alSourcei(sounds[SOUND_MUSIC].source, AL_LOOPING, 1);
    alSourcef(sounds[SOUND_MUSIC].source, AL_GAIN, 0.5);



    

}