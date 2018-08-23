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

static const PadIndex kPadCount = 32;

class CircuitView {
public:
  CircuitView() {
    for (PadIndex i = 0; i < kPadCount; ++i) {
      Pad *pad = new Pad(i);
      _pads.push_back(pad);
    }
  }
  ~CircuitView() {
    for (Pad *pad : _pads) {
      delete pad;
    }
  }
  std::vector<Pad *> GetPads(const PadIndex &start, const PadIndex &count) {
    std::vector<Pad *> pads;
    for (PadIndex i = 0; i < count; ++i) {
      pads.push_back(_pads[i + start]);
    }
    return pads;
  }
  Pad *GetPad(const PadIndex &index) {
    return _pads[index];
  }
private:
  std::vector<Pad *> _pads;
};
#endif /* CircuitView_hpp */
