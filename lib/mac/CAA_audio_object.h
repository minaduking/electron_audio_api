//
// Created by ryominaduki on 2017/07/31.
//

#ifndef ELECTRON_AUDIO_API_CAA_AUDIO_OBJECT_H
#define ELECTRON_AUDIO_API_CAA_AUDIO_OBJECT_H

#import <Foundation/Foundation.h>
#import <iostream>
#import <CoreAudio/CoreAudio.h>
#import "CAA_audio_object_interface.h"

@interface CAAAudioObject: NSObject
- (CFArrayRef)createInputDeviceList;
- (CFArrayRef)createOutputDeviceList;
@end

#endif //ELECTRON_AUDIO_API_CAA_AUDIO_OBJECT_H
