//
//  GateView.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/24/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "SynthGateView.hpp"

static const Color kZeroGateColor(0x00000000);
static const ColorCode kFullGateColorCode = 0x000000ff;
static const Color kFullGateColor(kFullGateColorCode);
static const uint8_t kGateIncrementPerPad = 8;

SynthGateView::SynthGateView(const std::vector<Pad *> &pads, SynthGateViewDelegate *delegate):
_pads(pads), _delegate(delegate) {
  assert(pads.size() == kDiscreteGateValues / kGateIncrementPerPad);
  for (PadIndex i = 0; i < pads.size(); ++i) {
    Pad *pad = _pads[i];
    _pad_index_mapping[pad->GetPadIndex()] = i;
    pad->SetDelegate(this);
  }
}

void SynthGateView::SetGate(const Gate &gate) {
  _previous_gate = gate;
  for (PadIndex i = 0; i < _pads.size(); ++i ){
    Pad *pad = _pads[i];
    PadIndex max_pad = gate / kGateIncrementPerPad;
    if (i < max_pad) {
      pad->SetColor(kFullGateColor);
    } else if (i == max_pad) {
      static const ColorCode increment_color_per_gate_value =
      (kFullGateColorCode + 1) / kGateIncrementPerPad;
      ColorCode code = increment_color_per_gate_value * (gate % kGateIncrementPerPad + 1) - 1;
      pad->SetColor(Color(code));
    } else {
      pad->SetColor(kZeroGateColor);
    }
  }
}

void SynthGateView::Tap(Pad *pad) {
  PadIndex pad_index = pad->GetPadIndex();
  PadIndex previous_pad = _previous_gate / kGateIncrementPerPad;
  Gate gate = 0;
  if (pad_index != previous_pad) {
    gate = (1 + pad_index) * kGateIncrementPerPad - 1;
  } else {
    ++_previous_gate;
    if (_previous_gate % kGateIncrementPerPad) {
      _previous_gate -= kGateIncrementPerPad;
    }
    gate = _previous_gate;
  }
  SetGate(gate);
  if (_delegate) {
    _delegate->Tap(gate);
  }
}

void SynthGateView::Release(Pad *pad) {
  // no-op.
}
