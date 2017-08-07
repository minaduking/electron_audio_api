#import "CAA_audio_object.h"

using namespace std;

typedef struct{
    AudioObjectPropertyAddress address;
    AudioDeviceID *audioDevices;
    UInt32 audioDeviceCount;
    OSStatus status;
    UInt32 dataSize;
    CFMutableArrayRef deviceArray;
} AudioIODeviceProperty;

AUDIO_SOURCES AudioGetInputDeviceSources(){
    CoreAudioObject *obj = [[CoreAudioObject alloc] init];
    NSArray* audioDeviceInputList = (__bridge NSArray*)[obj createInputDeviceList];
    [obj release];
    
    AUDIO_SOURCES audio_sources;
    for (NSUInteger i = 0; i < [audioDeviceInputList count]; i++){
        NSString *deviceName = [[audioDeviceInputList objectAtIndex:i] objectForKey:@"deviceName"];
        NSString *deviceUID = [[audioDeviceInputList objectAtIndex:i] objectForKey:@"deviceUID"];

        AUDIO_SOURCE audio_source;
        audio_source.name = string([deviceName UTF8String]);
        audio_source.uid = string([deviceUID UTF8String]);
        audio_sources.audio_source[i] = audio_source;
    }
    return audio_sources;
}

AUDIO_SOURCES AudioGetOutputDeviceSources(){
    CoreAudioObject *obj = [[CoreAudioObject alloc] init];
    NSArray* audioDeviceOutputList = (__bridge NSArray*)[obj createOutputDeviceList];
    [obj release];
    
    AUDIO_SOURCES audio_sources;
    for (NSUInteger i = 0; i < [audioDeviceOutputList count]; i++){
        NSString *deviceName = [[audioDeviceOutputList objectAtIndex:i] objectForKey:@"deviceName"];
        NSString *deviceUID = [[audioDeviceOutputList objectAtIndex:i] objectForKey:@"deviceUID"];

        AUDIO_SOURCE audio_source;
        audio_source.name = string([deviceName UTF8String]);
        audio_source.uid = string([deviceUID UTF8String]);
        audio_sources.audio_source[i] = audio_source;
    }
    return audio_sources;
}

@implementation CAAAudioObject

- (CFArrayRef)createInputDeviceList{
    AudioIODeviceProperty audioIODevice = [self createAudioIODevice];
    audioIODevice.address.mScope = kAudioDevicePropertyScopeInput;
    for(UInt32 i = 0; i < audioIODevice.audioDeviceCount; ++i) {
        
        // Query device input stream
        audioIODevice.address.mSelector = kAudioDevicePropertyStreams;
        audioIODevice.status = AudioObjectGetPropertyDataSize(audioIODevice.audioDevices[i], &audioIODevice.address, 0, NULL, &audioIODevice.dataSize);
        if(kAudioHardwareNoError != audioIODevice.status || audioIODevice.dataSize/sizeof(AudioStreamID) == 0) {
            fprintf(stderr, "AudioObjectGetPropertyData (kAudioDevicePropertyDeviceUID) failed: %i\n", audioIODevice.status);
            continue;
        }
        
        // Query device UID
        CFStringRef deviceUID = NULL;
        audioIODevice.dataSize = sizeof(deviceUID);
        audioIODevice.address.mSelector = kAudioDevicePropertyDeviceUID;
        audioIODevice.status = AudioObjectGetPropertyData(audioIODevice.audioDevices[i], &audioIODevice.address, 0, NULL, &audioIODevice.dataSize, &deviceUID);
        if(kAudioHardwareNoError != audioIODevice.status) {
            fprintf(stderr, "AudioObjectGetPropertyData (kAudioDevicePropertyDeviceUID) failed: %i\n", audioIODevice.status);
            continue;
        }
        
        // Query device name
        CFStringRef deviceName = NULL;
        audioIODevice.dataSize = sizeof(deviceName);
        audioIODevice.address.mSelector = kAudioDevicePropertyDeviceNameCFString;
        audioIODevice.status = AudioObjectGetPropertyData(audioIODevice.audioDevices[i], &audioIODevice.address, 0, NULL, &audioIODevice.dataSize, &deviceName);
        if(kAudioHardwareNoError != audioIODevice.status) {
            fprintf(stderr, "AudioObjectGetPropertyData (kAudioDevicePropertyDeviceNameCFString) failed: %i\n", audioIODevice.status);
            continue;
        }

        // Determine if the device is an input device (it is an input device if it has input channels)
        audioIODevice.dataSize = 0;
        audioIODevice.address.mSelector = kAudioDevicePropertyStreamConfiguration;
        audioIODevice.status = AudioObjectGetPropertyDataSize(audioIODevice.audioDevices[i], &audioIODevice.address, 0, NULL, &audioIODevice.dataSize);
        if(kAudioHardwareNoError != audioIODevice.status) {
            fprintf(stderr, "AudioObjectGetPropertyDataSize (kAudioDevicePropertyStreamConfiguration) failed: %i\n", audioIODevice.status);
            continue;
        }
        
        AudioBufferList *bufferList = static_cast<AudioBufferList *>(malloc(audioIODevice.dataSize));
        if(NULL == bufferList) {
            fputs("Unable to allocate memory", stderr);
            break;
        }
        
        audioIODevice.status = AudioObjectGetPropertyData(audioIODevice.audioDevices[i], &audioIODevice.address, 0, NULL, &audioIODevice.dataSize, bufferList);
        if(kAudioHardwareNoError != audioIODevice.status || 0 == bufferList->mNumberBuffers) {
            if(kAudioHardwareNoError != audioIODevice.status)
                fprintf(stderr, "AudioObjectGetPropertyData (kAudioDevicePropertyStreamConfiguration) failed: %i\n", audioIODevice.status);
            free(bufferList), bufferList = NULL;
            continue;
        }
        
        free(bufferList), bufferList = NULL;
        
        // Add a dictionary for this device to the array of input devices
        CFStringRef keys    []  = { CFSTR("deviceUID"),     CFSTR("deviceName") };
        CFStringRef values  []  = { deviceUID,              deviceName };
        CFDictionaryRef deviceDictionary = CFDictionaryCreate(kCFAllocatorDefault,
                                                      reinterpret_cast<const void **>(keys),
                                                      reinterpret_cast<const void **>(values),
                                                      2,
                                                      &kCFTypeDictionaryKeyCallBacks,
                                                      &kCFTypeDictionaryValueCallBacks);
        CFArrayAppendValue(audioIODevice.deviceArray, deviceDictionary);
        CFRelease(deviceDictionary), deviceDictionary = NULL;
    }
    
    free(audioIODevice.audioDevices), audioIODevice.audioDevices = NULL;
    
    // Return a non-mutable copy of the array
    CFArrayRef copy = CFArrayCreateCopy(kCFAllocatorDefault, audioIODevice.deviceArray);
    CFRelease(audioIODevice.deviceArray), audioIODevice.deviceArray = NULL;
    
    return copy;
}

- (CFArrayRef)createOutputDeviceList{
    AudioIODeviceProperty audioIODevice = [self createAudioIODevice];
    audioIODevice.address.mScope = kAudioDevicePropertyScopeOutput;
    for(UInt32 i = 0; i < audioIODevice.audioDeviceCount; ++i) {
        // Query device input stream
        audioIODevice.address.mSelector = kAudioDevicePropertyStreams;
        audioIODevice.status = AudioObjectGetPropertyDataSize(audioIODevice.audioDevices[i], &audioIODevice.address, 0, NULL, &audioIODevice.dataSize);
        if(kAudioHardwareNoError != audioIODevice.status || audioIODevice.dataSize/sizeof(AudioStreamID) == 0) {
            fprintf(stderr, "AudioObjectGetPropertyData (kAudioDevicePropertyDeviceUID) failed: %i\n", audioIODevice.status);
            continue;
        }
        
        // Query device UID
        CFStringRef deviceUID = NULL;
        audioIODevice.dataSize = sizeof(deviceUID);
        audioIODevice.address.mSelector = kAudioDevicePropertyDeviceUID;
        audioIODevice.status = AudioObjectGetPropertyData(audioIODevice.audioDevices[i], &audioIODevice.address, 0, NULL, &audioIODevice.dataSize, &deviceUID);
        if(kAudioHardwareNoError != audioIODevice.status) {
            fprintf(stderr, "AudioObjectGetPropertyData (kAudioDevicePropertyDeviceUID) failed: %i\n", audioIODevice.status);
            continue;
        }
        
        // Query device name
        CFStringRef deviceName = NULL;
        audioIODevice.dataSize = sizeof(deviceName);
        audioIODevice.address.mSelector = kAudioDevicePropertyDeviceNameCFString;
        audioIODevice.status = AudioObjectGetPropertyData(audioIODevice.audioDevices[i], &audioIODevice.address, 0, NULL, &audioIODevice.dataSize, &deviceName);
        if(kAudioHardwareNoError != audioIODevice.status) {
            fprintf(stderr, "AudioObjectGetPropertyData (kAudioDevicePropertyDeviceNameCFString) failed: %i\n", audioIODevice.status);
            continue;
        }
        
        // Determine if the device is an input device (it is an input device if it has input channels)
        audioIODevice.dataSize = 0;
        audioIODevice.address.mSelector = kAudioDevicePropertyStreamConfiguration;
        audioIODevice.status = AudioObjectGetPropertyDataSize(audioIODevice.audioDevices[i], &audioIODevice.address, 0, NULL, &audioIODevice.dataSize);
        if(kAudioHardwareNoError != audioIODevice.status) {
            fprintf(stderr, "AudioObjectGetPropertyDataSize (kAudioDevicePropertyStreamConfiguration) failed: %i\n", audioIODevice.status);
            continue;
        }
        
        AudioBufferList *bufferList = static_cast<AudioBufferList *>(malloc(audioIODevice.dataSize));
        if(NULL == bufferList) {
            fputs("Unable to allocate memory", stderr);
            break;
        }
        
        audioIODevice.status = AudioObjectGetPropertyData(audioIODevice.audioDevices[i], &audioIODevice.address, 0, NULL, &audioIODevice.dataSize, bufferList);
        if(kAudioHardwareNoError != audioIODevice.status || 0 == bufferList->mNumberBuffers) {
            if(kAudioHardwareNoError != audioIODevice.status)
                fprintf(stderr, "AudioObjectGetPropertyData (kAudioDevicePropertyStreamConfiguration) failed: %i\n", audioIODevice.status);
            free(bufferList), bufferList = NULL;
            continue;
        }
        
        free(bufferList), bufferList = NULL;
        
        // Add a dictionary for this device to the array of input devices
        CFStringRef keys    []  = { CFSTR("deviceUID"),     CFSTR("deviceName") };
        CFStringRef values  []  = { deviceUID,              deviceName };
        CFDictionaryRef deviceDictionary = CFDictionaryCreate(kCFAllocatorDefault,
                                                      reinterpret_cast<const void **>(keys),
                                                      reinterpret_cast<const void **>(values),
                                                      2,
                                                      &kCFTypeDictionaryKeyCallBacks,
                                                      &kCFTypeDictionaryValueCallBacks);

        
        CFArrayAppendValue(audioIODevice.deviceArray, deviceDictionary);
        
        CFRelease(deviceDictionary), deviceDictionary = NULL;
    }
    
    free(audioIODevice.audioDevices), audioIODevice.audioDevices = NULL;
    
    // Return a non-mutable copy of the array
    CFArrayRef copy = CFArrayCreateCopy(kCFAllocatorDefault, audioIODevice.deviceArray);
    CFRelease(audioIODevice.deviceArray), audioIODevice.deviceArray = NULL;
    
    return copy;
}

- (AudioIODeviceProperty)createAudioIODevice{
    AudioIODeviceProperty audioIODevice;

    AudioObjectPropertyAddress propertyAddress = {
        kAudioHardwarePropertyDevices,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMaster
    };
    
    UInt32 dataSize = 0;
    OSStatus status = AudioObjectGetPropertyDataSize(kAudioObjectSystemObject, &propertyAddress, 0, NULL, &dataSize);
    if(kAudioHardwareNoError != status) {
        fprintf(stderr, "AudioObjectGetPropertyDataSize (kAudioHardwarePropertyDevices) failed: %i\n", status);
        return audioIODevice;
    }
    
    UInt32 deviceCount = static_cast<UInt32>(dataSize / sizeof(AudioDeviceID));
    
    AudioDeviceID *audioDevices = static_cast<AudioDeviceID *>(malloc(dataSize));
    if(NULL == audioDevices) {
        fputs("Unable to allocate memory", stderr);
        return audioIODevice;
    }
    
    status = AudioObjectGetPropertyData(kAudioObjectSystemObject, &propertyAddress, 0, NULL, &dataSize, audioDevices);
    if(kAudioHardwareNoError != status) {
        fprintf(stderr, "AudioObjectGetPropertyData (kAudioHardwarePropertyDevices) failed: %i\n", status);
        free(audioDevices), audioDevices = NULL;
        return audioIODevice;
    }
    
    CFMutableArrayRef deviceArray = CFArrayCreateMutable(kCFAllocatorDefault, deviceCount, &kCFTypeArrayCallBacks);
    if(NULL == deviceArray) {
        fputs("CFArrayCreateMutable failed", stderr);
        free(audioDevices), audioDevices = NULL;
        return audioIODevice;
    }

    audioIODevice.address = propertyAddress;
    audioIODevice.audioDevices = audioDevices;
    audioIODevice.audioDeviceCount = deviceCount;
    audioIODevice.status = status;
    audioIODevice.dataSize = dataSize;
    audioIODevice.deviceArray = deviceArray;
    return audioIODevice;
}

@end
