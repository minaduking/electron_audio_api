//
// Created by ryominaduki on 2017/07/31.
//

#ifndef ELECTRON_AUDIO_API_AUDIO_SOURCE_H
#define ELECTRON_AUDIO_API_AUDIO_SOURCE_H

#include <iostream>

using namespace std;

struct AUDIO_SOURCE{
    string name;
    string uid;
};

struct AUDIO_SOURCES{
    AUDIO_SOURCE audio_source[64];
};

#endif //ELECTRON_AUDIO_API_AUDIO_SOURCE_H
