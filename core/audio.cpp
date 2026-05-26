#include "audio.h"

Audio::Audio(const char* path){

    if(SDL_LoadWAV(path, &this->spec, &this->buffer, &this->length) == NULL)
    {
        printf("Erro carregando WAV: %s\n", SDL_GetError());
        return;
    }

}

void Audio::play(){

    float volumeLeft = 1.0;
    float volumeRight = 1.0;

    SDL_AudioCVT cvt;

    SDL_BuildAudioCVT(
        &cvt,
        this->spec.format,
        this->spec.channels,
        this->spec.freq,

        AUDIO_F32SYS,
        2,
        44100
    );

    cvt.len = this->length;
    cvt.buf = (Uint8*)SDL_malloc(cvt.len * cvt.len_mult);

    SDL_memcpy(cvt.buf, this->buffer, this->length);

    SDL_ConvertAudio(&cvt);

    float* samples = (float*)cvt.buf;

    int sampleCount = cvt.len_cvt / sizeof(float);

    // stereo = L R L R
    for(int i = 0; i < sampleCount; i += 2)
    {
        samples[i] *= volumeLeft;
        if(i + 1 < sampleCount) samples[i + 1] *= volumeRight;
    }

    SDL_QueueAudio( this->audioDevice, cvt.buf, cvt.len_cvt );

    SDL_free(cvt.buf);
}

Audio::~Audio(){
    
}

AudioManager::AudioManager(){
    
    audios = new Audio*[this->maxNAudios];

    SDL_AudioSpec desiredSpec;
    SDL_zero(desiredSpec);
    desiredSpec.freq = 44100;
    desiredSpec.format = AUDIO_F32SYS; // float
    desiredSpec.channels = 2;          // stereo
    desiredSpec.samples = 2048;
    desiredSpec.callback = NULL;       // queue audio

    audioDevice = SDL_OpenAudioDevice(
        NULL,
        0,
        &desiredSpec,
        &obtainedSpec,
        0
    );

    if(audioDevice == 0)
    {
        printf("Error to initiate audio manager: %s\n", SDL_GetError());
        return;
    }

    SDL_PauseAudioDevice(audioDevice, 0);
}

void AudioManager::addElement(Audio *audio){
    this->audios[nAudios++] = audio;
    audio->audioDevice = this->audioDevice;
}

AudioManager::~AudioManager(){

}

