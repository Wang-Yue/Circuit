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
  virtual void ProgramChange(const SynthIndex &index) override {
    MidiController::GetInstance().SendProgramChange(_channel_index, index);
  }
  virtual void ControlChange(const Control &index, const CC &cc) override {
    MidiController::GetInstance().SendControlMessage(_channel_index, index, cc);
  }
private:
  const ChannelIndex _channel_index;
};

#include "SynthMain.h"

class DX7SynthChannelOutput : public SynthChannelOutputInterface {
public:
  DX7SynthChannelOutput(const ChannelIndex &channel_index) : _channel_index(channel_index) {
    _synth_main = new SynthMain();
    _synth_main->SynthInit();
  }
  virtual void NoteOn(const MIDINote &note, const Velocity &velocity) override {
    
    _synth_main->SendPlayNoteMessage(_channel_index, note, velocity);
  }
  virtual void NoteOff(const MIDINote &note) override {
    _synth_main->SendStopNoteMessage(_channel_index, note);
  }
  virtual void ProgramChange(const SynthIndex &index) override {
    _synth_main->SendProgramChange(_channel_index, index);
  }
  virtual void ControlChange(const Control &index, const CC &cc) override {
    _synth_main->SendControlChange(_channel_index, index, cc);
  }
private:
  const ChannelIndex _channel_index;
  SynthMain *_synth_main;
};

class MIDISampleChannelOutput : public SampleChannelOutputInterface {
public:
  MIDISampleChannelOutput(const ChannelIndex &channel_index) : _channel_index(channel_index) {
  }
  virtual void Play(const Velocity &velocity, const SampleIndex &sample_index) override {
    MidiController::GetInstance().SendPlayNoteMessage(_channel_index, sample_index, velocity);
  }
  virtual void ControlChange(const Control &index, const CC &cc) override {
    MidiController::GetInstance().SendControlMessage(_channel_index, index, cc);
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
    //    _synth_output.push_back(new MIDISynthChannelOutput(i));
    _synth_output.push_back(new DX7SynthChannelOutput(i));
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


