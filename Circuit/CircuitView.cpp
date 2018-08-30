//
//  CircuitView.cpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "CircuitView.hpp"

#include "Pad.hpp"
#include "Knob.hpp"

CircuitView::CircuitView() {
  for (PadIndex i = 0; i < PadUnknown; ++i) {
    Pad *pad = new Pad(i);
    _pads.push_back(pad);
  }
  for (KnobIndex i = 0; i < kKnobCount; ++i) {
    Knob *knob = new Knob(i);
    _knobs.push_back(knob);
  }
}

CircuitView::~CircuitView() {
  for (Pad *pad : _pads) {
    delete pad;
  }
  for (Knob *knob : _knobs) {
    delete knob;
  }
}

std::vector<Pad *> CircuitView::GetRegularPads(const PadIndex &start, const PadIndex &count) {
  assert(count <= kRegularPadCount);
  std::vector<Pad *> pads;
  for (PadIndex i = PadRegular; i < PadRegular + count; ++i) {
    pads.push_back(_pads[i + start]);
  }
  return pads;
}

Pad *CircuitView::GetPad(const PadIndex &index) {
  return _pads[index];
}

Pad *CircuitView::GetControlPad(const PadIndex &index) {
  assert(index >= kRegularPadCount);
  assert(index < PadUnknown);
  return _pads[index];
}

std::vector<Knob *> CircuitView::GetKnobs() {
  return _knobs;
}

Knob *CircuitView::GetKnob(const KnobIndex &index) const {
  assert(index < kKnobCount);
  return _knobs[index];
}
