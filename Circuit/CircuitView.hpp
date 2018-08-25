//
//  CircuitView.hpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef CircuitView_hpp
#define CircuitView_hpp

#include "Pad.hpp"

#include <vector>


static const PadIndex kRegularPadCount = 32;

enum CircuitPad : PadIndex {
  PadRegular = 0,
  PadNote = kRegularPadCount,
  PadGate,
  PadVelocity,
  PadNudge,
  PadLength,
  PadScale,
  PadOctUp,
  PadOctDown,
  PadTempo,
  PadSwing,
  PadClear,
  PadDuplicate,
  PadSave,
  PadSessions,
  PadShift,
  PadPatterns,
  PadMixer,
  PadFX,
  PadRecord,
  PadPlay,
  PadSynth1,
  PadSynth2,
  PadDrum1,
  PadDrum2,
  PadDrum3,
  PadDrum4,
  PadUnknown,
};

class CircuitView {
public:
  CircuitView() {
    for (PadIndex i = 0; i < PadUnknown; ++i) {
      Pad *pad = new Pad(i);
      _pads.push_back(pad);
    }
  }
  ~CircuitView() {
    for (Pad *pad : _pads) {
      delete pad;
    }
  }
  std::vector<Pad *> GetRegularPads(const PadIndex &start, const PadIndex &count) {
    assert(count <= kRegularPadCount);
    std::vector<Pad *> pads;
    for (PadIndex i = PadRegular; i < PadRegular + count; ++i) {
      pads.push_back(_pads[i + start]);
    }
    return pads;
  }
  
  Pad *GetPad(const PadIndex &index) {
    return _pads[index];
  }
  
  Pad *GetControlPad(const PadIndex &index) {
    assert(index >= kRegularPadCount);
    assert(index < PadUnknown);
    return _pads[PadNote];
  }

private:
  std::vector<Pad *> _pads;
};
#endif /* CircuitView_hpp */
