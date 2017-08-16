#include <nan.h>
#include "audio.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;

void init(v8::Local<v8::Object> exports){
    Audio::Init(exports);
}

NODE_MODULE(electron_audio_api, init);
