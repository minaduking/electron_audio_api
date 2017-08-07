#include <nan.h>
#include <iostream>
#include "audio.h"

using v8::Uint8ClampedArray;
using v8::Local;

Audio::Audio() {}
Audio::~Audio() {}

AUDIO_SOURCES Audio::getInputSources() {
    AUDIO_SOURCES audio_sources;
#ifdef _WIN32
    audio_sources = AudioGetInputDeviceSources();
#elif __APPLE__
#   ifdef TARGET_OS_MAC
    audio_sources = AudioGetInputDeviceSources();
#   endif
#else
//        audio_sources->audio_source[0].name = "other audio source";
#endif
    return audio_sources;
}

AUDIO_SOURCES Audio::getOutputSources() {
    AUDIO_SOURCES audio_sources;
#ifdef _WIN32
    audio_sources = AudioGetOutputDeviceSources();
#elif __APPLE__
#   ifdef TARGET_OS_MAC
    audio_sources = AudioGetOutputDeviceSources();
#   endif
#else
//        audio_sources->audio_source[0].name = "other audio source";
#endif
    return audio_sources;
}

NAN_METHOD(GetInputAudioSources){
    info.GetReturnValue().Set(Nan::New<AUDIO_SOURCES>(Audio::getInputSources()).ToLocalChecked());
}

NAN_METHOD(GetOutputAudioSources){
    info.GetReturnValue().Set(Nan::New(Audio::getOutputSources().audio_source[0].name).ToLocalChecked());
}