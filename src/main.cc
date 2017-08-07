#include <nan.h>
#include "audio.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;

NAN_MODULE_INIT(init){
//    Set(target, New<String>("getAudioSample").ToLocalChecked(), GetFunction(New<FunctionTemplate>(GetAudioSample)).ToLocalChecked());
    Set(target, New<String>("getInputAudioSources").ToLocalChecked(), GetFunction(New<FunctionTemplate>(GetInputAudioSources)).ToLocalChecked());
    Set(target, New<String>("getOutputAudioSources").ToLocalChecked(), GetFunction(New<FunctionTemplate>(GetOutputAudioSources)).ToLocalChecked());
}

NODE_MODULE(electron_audio_api, init);
