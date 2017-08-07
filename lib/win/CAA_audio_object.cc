#include "CAA_audio_object.h"
#include <string>

AUDIO_SOURCES AudioGetInputDeviceSources(){
    CAAAudio* audio = new CAAAudio();
    AUDIO_SOURCES audio_sources = audio->listAudioDevices();
    return audio_sources;
}

AUDIO_SOURCES AudioGetOutputDeviceSources(){
    CAAAudio* audio = new CAAAudio();
    AUDIO_SOURCES audio_sources = audio->listAudioDevices();
    return audio_sources;
}