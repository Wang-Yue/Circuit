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
  virtual void NoteOn(const Note &note, const Velocity &velocity) = 0;
  virtual void NoteOff(const Note &note) = 0;
};

#endif /* MIDIDelegate_hpp */
