// AVFoundationWrapper.mm

#import "Wrapper.h"
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>


double MyClass::secondsSince1970()
{
  return [[NSDate date] timeIntervalSince1970];
}

void delay(int seconds) {
    [NSThread sleepForTimeInterval:seconds];
}

void MyClass::playAudio(const char* filePath) {
    @autoreleasepool {
        /* Use this code to play an audio file */
        NSString *soundFilePath = [[NSBundle mainBundle] pathForResource:@"test"  ofType:@"mp3"];
        NSURL *soundFileURL = [NSURL fileURLWithPath:soundFilePath];

        NSError *error = nil;


        AVAudioPlayer * audioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:soundFileURL error:&error];
        if (!audioPlayer) {
            NSLog(@"Error initializing audio player: %@", [error localizedDescription]);
            return;
        }

        [(AVAudioPlayer *)audioPlayer prepareToPlay];
        [(AVAudioPlayer *)audioPlayer play];
        delay(40); // Delay for 10 seconds

    }
}