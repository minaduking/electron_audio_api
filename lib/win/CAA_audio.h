#ifndef ELECTRON_AUDIO_API_CAA_AUDIO_H
#define ELECTRON_AUDIO_API_CAA_AUDIO_H


#include <mmdeviceapi.h>
// #include <audioclient.h>
#include <functiondiscoverykeys_devpkey.h>
#include <locale.h>
#include <stdio.h>
#include "../common/struct/audio_source.h"

class CAAAudio{
    public:
        CAAAudio();
        ~CAAAudio();
        AUDIO_SOURCES listAudioDevices();
};

#endif //ELECTRON_AUDIO_API_CAA_AUDIO_H