//
//  ChannelOutputInterface.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/26/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef ChannelOutputInterface_hpp
#define ChannelOutputInterface_hpp

#include "TypeDefs.hpp"

class SynthChannelOutputInterface {
public:
  virtual void NoteOn(const MIDINote &note, const Velocity &velocity) = 0;
  virtual void NoteOff(const MIDINote &note) = 0;
  virtual void ProgramChange(const SynthIndex &index) = 0;
  virtual void ControlChange(const Control &index, const CC &cc) = 0;
};

class SampleChannelOutputInterface {
public:
  virtual void Play(const Velocity &velocity, const SampleIndex &sample_index) = 0;
  virtual void ControlChange(const Control &index, const CC &cc) = 0;
};

#endif /* ChannelOutputInterface_hpp */
