#include "CAA_audio.h"
#include <iostream>

CAAAudio::CAAAudio(){}
CAAAudio::~CAAAudio(){}

AUDIO_SOURCES CAAAudio::listAudioDevices()
{
    AUDIO_SOURCES audio_sources;


    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr)){
		printf("Unable to initialize COM: %x\n", hr);
        return audio_sources;
	}

    IMMDeviceEnumerator *deviceEnumerator;
    // hr = ::CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceEnumerator));
    hr = ::CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&deviceEnumerator);
    if (FAILED(hr)){
        printf("CoCreateInstance(IMMDeviceEnumerator) failed: hr = 0x%08x\n", hr);
        return audio_sources;
	}

    IMMDeviceCollection *deviceCollection;
    hr = deviceEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &deviceCollection);
    // hr = deviceEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATEMASK_ALL, &deviceCollection);
    deviceEnumerator->Release();
    if (FAILED(hr)){
		printf("IMMDeviceEnumerator::EnumAudioEndpoints failed: hr = 0x%08x\n", hr);
        return audio_sources;
	}

    UINT deviceCount;
    hr = deviceCollection->GetCount(&deviceCount);
    if (FAILED(hr)){
		printf("IMMDeviceCollection::GetCount failed: hr = 0x%08x\n", hr);
        return audio_sources;
	}
    printf("Active render endpoints found: %u\n", deviceCount);

    for (UINT i = 0; i < deviceCount; i++){
        IMMDevice *device;
        hr = deviceCollection->Item(i, &device);
        if (FAILED(hr)) {
            deviceCollection->Release();
            printf("IMMDeviceCollection::Item failed: hr = 0x%08x\n", hr);
            return audio_sources;
        }

        IPropertyStore *propertyStore;
        hr = device->OpenPropertyStore(STGM_READ, &propertyStore);
        if(FAILED(hr)){
            deviceCollection->Release();
            printf("IMMDevice::OpenPropertyStore failed: hr = 0x%08x\n", hr);
            return audio_sources;
        }

        PROPVARIANT pv;
        PropVariantInit(&pv);
        hr = propertyStore->GetValue(PKEY_Device_FriendlyName, &pv);
        propertyStore->Release();
        if (FAILED(hr)) {
            deviceCollection->Release();
            printf("IPropertyStore::GetValue failed: hr = 0x%08x\n", hr);
            return audio_sources;
        }

        if(VT_LPWSTR != pv.vt){
            deviceCollection->Release();
            printf("PKEY_Device_FriendlyName variant type is %u - expected VT_LPWSTR", pv.vt);
            PropVariantClear(&pv);
            return audio_sources;
        }
        setlocale(LC_ALL, setlocale(LC_CTYPE, ""));

        char    deviceName[256];
        size_t  ret_deviceName;
        wcstombs_s(&ret_deviceName, deviceName, 256, pv.pwszVal, _TRUNCATE);

        LPWSTR id;
        hr = device->GetId(&id);
        if(FAILED(hr)) {
            deviceCollection->Release();
            printf("IMMDevice::GetId failed: hr = 0x%08x\n", hr);
            return audio_sources;
        }
        char    deviceId[256];
        size_t  ret_deviceId;
        wcstombs_s(&ret_deviceId, deviceId, 256, id, _TRUNCATE);

        AUDIO_SOURCE audio_source;
        audio_source.name = deviceName;
        audio_source.uid = deviceId;
        audio_sources.audio_source[i] = audio_source;

        PropVariantClear(&pv);
        device->Release();
	}
    deviceCollection->Release();

    return audio_sources; 
}