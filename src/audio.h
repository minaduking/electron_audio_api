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

using namespace v8;

class Audio: public Nan::ObjectWrap {
public:
    static void Init(Local<Object> exports);

private:
    explicit Audio();
    ~Audio();

    static Nan::Persistent<Function> constructor;
    static void New(const Nan::FunctionCallbackInfo<Value>& info);

    static void getInputSources(const Nan::FunctionCallbackInfo<Value>& info);
    static void getOutputSources(const Nan::FunctionCallbackInfo<Value>& info);
};
NAN_METHOD(CreateObject);
#endif //ELECTRON_AUDIO_API_AUDIO_H
