#pragma once
#include <SDL2/SDL.h>
#include "vec3.h"
#include "camera.h"

class Audio {

    public:

        float* samples = nullptr;
        Uint32 sampleCount = 0;

        Uint32 position = 0;
        Vec3 *pos = nullptr;

        float volumeLeft = 1.0f;
        float volumeRight = 1.0f;

        bool playing = false;

        Audio(const char* path);
        ~Audio();

        void play();
        void stop();

        void setPos(Vec3 *pos);
        
};

class AudioManager
{

    public:

        Camera *camera;
        SDL_AudioSpec obtainedSpec;
        SDL_AudioDeviceID audioDevice;

        int nAudios = 0;
        int maxNAudios = 100;

        Audio **audios;

        AudioManager(Camera *camera);
        ~AudioManager();

        void setCamera(Camera *camera);

        void addElement(Audio *audio);

        static void audioCallback(void* userdata, Uint8* stream, int len);

};




