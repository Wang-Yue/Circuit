//
//  MIDIDelegate.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/27/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef MIDIDelegate_hpp
#define MIDIDelegate_hpp

#include "TypeDefs.hpp"

class MIDIDelegate {
public:
  virtual void NoteOn(const MIDINote &note, const Velocity &velocity) = 0;
  virtual void NoteOff(const MIDINote &note) = 0;
  virtual void SendCC(const Control &control, const CC &cc) = 0;
};

#endif /* MIDIDelegate_hpp */
