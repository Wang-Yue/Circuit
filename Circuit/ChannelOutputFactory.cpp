//
//  ChannelOutputFactory.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/26/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "ChannelOutputFactory.hpp"

#include "ChannelOutputInterface.hpp"
#include "MidiController.hpp"

class MIDISynthChannelOutput : public SynthChannelOutputInterface {
public:
  MIDISynthChannelOutput(const ChannelIndex &channel_index) : _channel_index(channel_index) {
  }
  virtual void NoteOn(const MIDINote &note, const Velocity &velocity) override {
    MidiController::GetInstance().SendPlayNoteMessage(_channel_index, note, velocity);
  }
  virtual void NoteOff(const MIDINote &note) override {
    MidiController::GetInstance().SendStopNoteMessage(_channel_index, note);
  }
private:
  const ChannelIndex _channel_index;
};

class MIDISampleChannelOutput : public SampleChannelOutputInterface {
public:
  MIDISampleChannelOutput(const ChannelIndex &channel_index) : _channel_index(channel_index) {
  }
  virtual void Play(const Velocity &velocity, const SampleIndex &sample_index) override {
    MidiController::GetInstance().SendPlayNoteMessage(_channel_index, sample_index, velocity);
  }
private:
  const ChannelIndex _channel_index;
};

ChannelOutputFactory& ChannelOutputFactory::GetInstance() {
  static ChannelOutputFactory instance;
  return instance;
}

ChannelOutputFactory::ChannelOutputFactory() {
  for (ChannelIndex i = 0; i < kSynthChannelsCapacity; ++i) {
    _synth_output.push_back(new MIDISynthChannelOutput(i));
  }
  for (ChannelIndex i = 0; i < kSampleChannelsCapacity; ++i) {
    _sample_output.push_back(new MIDISampleChannelOutput(i + kSynthChannelsCapacity));
  }
}
SynthChannelOutputInterface *ChannelOutputFactory::GetSynthChannelOutput(const ChannelIndex &index) {
  assert(index < kSynthChannelsCapacity);
  return _synth_output[index];
}
SampleChannelOutputInterface *ChannelOutputFactory::GetSampleChannelOutput(const ChannelIndex &index) {
  assert(index < kSampleChannelsCapacity);
  return _sample_output[index];
};


