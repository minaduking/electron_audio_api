//
// Created by ryominaduki on 2017/07/31.
//

#ifndef ELECTRON_AUDIO_API_AUDIO_H
#define ELECTRON_AUDIO_API_AUDIO_H

#include <nan.h>
#include "../lib/common/struct/audio_source.h"

#ifdef _WIN32
    #include "../lib/win/CAA_audio_object.h"
#elif __APPLE__
#   ifdef TARGET_OS_MAC
    #include "../lib/mac/CAA_audio_object_interface.h"
#   endif
#endif

class Audio {
public:
    Audio();
    ~Audio();

    static AUDIO_SOURCES getInputSources();
    static AUDIO_SOURCES getOutputSources();
};

NAN_METHOD(GetInputAudioSources);
NAN_METHOD(GetOutputAudioSources);

#endif //ELECTRON_AUDIO_API_AUDIO_H
