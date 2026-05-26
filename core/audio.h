#pragma once
#include <SDL2/SDL.h>

class Audio {

    public:

        Uint8* buffer;
        Uint32 length;
        SDL_AudioSpec spec;
        SDL_AudioDeviceID audioDevice;

        Audio(const char* path);
        ~Audio();

        void play();

};

class AudioManager
{

    public:

        SDL_AudioSpec obtainedSpec;
        SDL_AudioDeviceID audioDevice;

        int nAudios = 0;
        int maxNAudios = 10;
        Audio **audios;
        
        AudioManager();
        ~AudioManager();

        void addElement(Audio *audio);

};


