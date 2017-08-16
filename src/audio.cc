#include <nan.h>
#include <stdio.h>
#include "audio.h"

Nan::Persistent<v8::Function> Audio::constructor;

Audio::Audio() {}
Audio::~Audio() {}

void Audio::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Audio").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "getInputSources", getInputSources);
  Nan::SetPrototypeMethod(tpl, "getOutputSources", getOutputSources);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("AudioObject").ToLocalChecked(), tpl->GetFunction());
}

void Audio::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.IsConstructCall()) {
    Audio* obj = new Audio();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = { info[0] };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void Audio::getInputSources(const Nan::FunctionCallbackInfo<v8::Value>& info) {
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
    v8::Local<v8::Array> array = Nan::New<v8::Array>();
    size_t size = sizeof(audio_sources.audio_source) / sizeof(audio_sources.audio_source[0]);
    for(int i=0; i < (int)size; i++){
        v8::Local<v8::Object> obj = Nan::New<v8::Object>();
        obj->Set(Nan::New("name").ToLocalChecked(), Nan::New(audio_sources.audio_source[i].name).ToLocalChecked());
        obj->Set(Nan::New("uid").ToLocalChecked(), Nan::New(audio_sources.audio_source[i].uid).ToLocalChecked());
        Nan::Set(array, i, obj);
    }
    info.GetReturnValue().Set(array);
}

void Audio::getOutputSources(const Nan::FunctionCallbackInfo<v8::Value>& info) {
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
    v8::Local<v8::Array> array = Nan::New<v8::Array>();
    size_t size = sizeof(audio_sources.audio_source) / sizeof(audio_sources.audio_source[0]);
    for(int i=0; i < (int)size; i++){
        v8::Local<v8::Object> obj = Nan::New<v8::Object>();
        obj->Set(Nan::New("name").ToLocalChecked(), Nan::New(audio_sources.audio_source[i].name).ToLocalChecked());
        obj->Set(Nan::New("uid").ToLocalChecked(), Nan::New(audio_sources.audio_source[i].uid).ToLocalChecked());
        Nan::Set(array, i, obj);
    }
    info.GetReturnValue().Set(array);
}