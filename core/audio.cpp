#include "audio.h"
#include <stdio.h>
#include "camera.h"
#include "vec3.h"

Audio::Audio(const char* path)
{
    Uint8* wavBuffer;
    Uint32 wavLength;
    SDL_AudioSpec wavSpec;

    if(SDL_LoadWAV(path, &wavSpec, &wavBuffer, &wavLength) == NULL)
    {
        printf("Erro carregando WAV: %s\n", SDL_GetError());
        return;
    }

    SDL_AudioCVT cvt;

    SDL_BuildAudioCVT(
        &cvt,
        wavSpec.format,
        wavSpec.channels,
        wavSpec.freq,

        AUDIO_F32SYS,
        2,
        44100
    );

    cvt.len = wavLength;
    cvt.buf = (Uint8*)SDL_malloc(cvt.len * cvt.len_mult);

    SDL_memcpy(cvt.buf, wavBuffer, wavLength);

    SDL_ConvertAudio(&cvt);

    this->samples = (float*)cvt.buf;

    sampleCount = cvt.len_cvt / sizeof(float);

    SDL_FreeWAV(wavBuffer);
}

void Audio::play()
{
    position = 0;
    playing = true;
}

void Audio::stop()
{
    playing = false;
}

void Audio::setPos(Vec3 *pos){
    this->pos = pos;
}

Audio::~Audio()
{
    if(samples)
        SDL_free(samples);
}

AudioManager::AudioManager(Camera *camera)
{

    this->camera = camera;

    audios = new Audio*[maxNAudios];

    SDL_AudioSpec desiredSpec;

    SDL_zero(desiredSpec);

    desiredSpec.freq = 44100;
    desiredSpec.format = AUDIO_F32SYS;
    desiredSpec.channels = 2;
    desiredSpec.samples = 2048;

    desiredSpec.callback = AudioManager::audioCallback;
    desiredSpec.userdata = this;

    audioDevice = SDL_OpenAudioDevice(
        NULL,
        0,
        &desiredSpec,
        &obtainedSpec,
        0
    );

    if(audioDevice == 0)
    {
        printf("Erro audio device: %s\n", SDL_GetError());
        return;
    }

    SDL_PauseAudioDevice(audioDevice, 0);
}

void AudioManager::audioCallback(void* userdata, Uint8* stream, int len)
{
    AudioManager* manager = (AudioManager*)userdata;

    float* out = (float*)stream;

    int sampleCount = len / sizeof(float);

    // limpar buffer
    for(int i = 0; i < sampleCount; i++)
        out[i] = 0.0f;

    // mix audios
    for(int a = 0; a < manager->nAudios; a++)
    {
        Audio* audio = manager->audios[a];
        
        if(audio->pos != nullptr){

            float dx = manager->camera->getX() - audio->pos->x;
            float dy = manager->camera->getY() - audio->pos->y;
            float dz = manager->camera->getZ() - audio->pos->z;
            float distance = sqrt(dx * dx + dy * dy + dz * dz);
            distance = distance > 100 ? 100 : distance;

            Vec3 soundDir(
                audio->pos->x - manager->camera->getX(),
                audio->pos->y - manager->camera->getY(),
                audio->pos->z - manager->camera->getZ()
            );

            soundDir = soundDir.versor();
            float yaw = manager->camera->hpr.y * M_PI / 180.0f;

            Vec3 forward(-sin(yaw),0,cos(yaw));
            Vec3 right(forward.z,0,-forward.x);
            float pan = soundDir.produto_escalar(right);

            if(pan < -1.0f) pan = -1.0f;
            if(pan > 1.0f)  pan = 1.0f;

            audio->volumeLeft  = (1.0f + pan) * 0.5f;
            audio->volumeRight = (1.0f - pan) * 0.5f;

        }

        if(!audio->playing)
            continue;

        for(int i = 0; i < sampleCount; i += 2)
        {
            if(audio->position >= audio->sampleCount)
            {
                audio->playing = false;
                break;
            }
 
            out[i] += audio->samples[audio->position++] * audio->volumeLeft;

            if(i + 1 < sampleCount && audio->position < audio->sampleCount)
            {
                out[i + 1] += audio->samples[audio->position++] * audio->volumeRight;
            }
        }
    }
}

void AudioManager::addElement(Audio *audio)
{
    audios[nAudios++] = audio;
}

void AudioManager::setCamera(Camera *camera){
    this->camera = camera;
}