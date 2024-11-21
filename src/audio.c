#include "audio.h"





void play_sound (Sound* sound){

    alSourcePlay(sound->source);

}

void stop_sound(Sound* sound){


    alSourceStop(sound->source);



}

void load_sounds(Sound sounds[10]){

    sounds[SOUND_TEST].buffer = alutCreateBufferFromFile("../assets/audio/jump.wav");
    alGenSources(1, &sounds[SOUND_TEST].source);
    alSourcei(sounds[SOUND_TEST].source, AL_BUFFER, sounds[SOUND_TEST].buffer);



    sounds[SOUND_MENU].buffer = alutCreateBufferFromFile("../assets/audio/snake_song_2.wav");
    alGenSources(1, &sounds[SOUND_MENU].source);
    alSourcei(sounds[SOUND_MENU].source, AL_BUFFER, sounds[SOUND_MENU].buffer);
    alSourcei(sounds[SOUND_MENU].source, AL_LOOPING, 1);
    alSourcef(sounds[SOUND_MENU].source, AL_GAIN, 0.5);



    

}