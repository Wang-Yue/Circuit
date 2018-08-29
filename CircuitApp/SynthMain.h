/*
 * Copyright 2012 Google Inc.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __SYNTH_SYNTH_MAIN_H
#define __SYNTH_SYNTH_MAIN_H

#include <AudioUnit/AudioUnit.h>

#include "synth.h"
#include "synth_unit.h"

class SynthMain {
 public:
  int SynthInit();
  int SynthDone();
  int Load(const char *filename);
  void SendPlayNoteMessage(const char &channel, const char &note, const char &velocity){
    uint8_t message[3];
    message[0] = 144 + channel;
    message[1] = note;
    message[2] = velocity;
    ring_buffer_.Write(message, 3);
    printf("Start note %d, %d\n", note, velocity);
  };
  void SendStopNoteMessage(const char &channel, const char &note){
    uint8_t message[3];
    message[0] = 128 + channel;
    message[1] = note;
    message[2] = 0;
    ring_buffer_.Write(message, 3);
    printf("Stop note %d\n", note);
  };
  void SendProgramChange(const char &channel, const char &program){
    uint8_t message[2];
    message[0] = 192 + channel;
    message[1] = program;
    ring_buffer_.Write(message, 2);
    printf("Program change %d\n", program);
  };
 private:
  OSStatus setupplayback(SynthUnit *synth_unit);
  OSStatus startplayback();
  OSStatus stopplayback();
  AudioUnit audioUnit_;
  RingBuffer ring_buffer_;
  SynthUnit *synth_unit_;
};

#endif  // __SYNTH_SYNTH_MAIN_H
