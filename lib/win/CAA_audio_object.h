//
// Created by ryominaduki on 2017/07/31.
//

#ifndef ELECTRON_AUDIO_API_CAA_AUDIO_OBJECT_H
#define ELECTRON_AUDIO_API_CAA_AUDIO_OBJECT_H

#include <windows.h>
#include "CAA_audio.h"
#include "../common/struct/audio_source.h"

AUDIO_SOURCES AudioGetInputDeviceSources();
AUDIO_SOURCES AudioGetOutputDeviceSources();

#endif //ELECTRON_AUDIO_API_CAA_AUDIO_OBJECT_H
