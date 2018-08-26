//
//  SampleGateView.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/26/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "SampleGateView.hpp"


#include "SampleGateView.hpp"

#include "Color.hpp"

static const Color kDisabledColor(0x00000000);
static const Color kAvailableColor(0x0000003f);
static const Color kEnabledColor(0x000000ff);

SampleGateView::SampleGateView(const std::vector<Pad *> &pads, SampleGateViewDelegate *delegate):
_pads(pads), _delegate(delegate) {
  assert(pads.size() == kStepCapacity);
  for (PadIndex i = 0; i < _pads.size(); ++i) {
    Pad *pad = _pads[i];
    if (i < kMicrosteps) {
      _pad_index_mapping[pad->GetPadIndex()] = 1 << i;
    }
    pad->SetDelegate(this);
  }
}

SampleGateView::~SampleGateView() {
  for (Pad *pad : _pads) {
    pad->SetDelegate(nullptr);
  }
}

void SampleGateView::SetMicrostepTicks(const Microstep &microstep_ticks) {
  if (_microstep_ticks == microstep_ticks) {
    return;
  }
  _microstep_ticks = microstep_ticks;
  for (PadIndex i = 0; i < _pads.size(); ++i ){
    Pad *pad = _pads[i];
    if (i > kMicrosteps) {
      pad->SetColor(kDisabledColor);
    } else {
      bool enabled = microstep_ticks & (1 << i);
      pad->SetColor(enabled ? kEnabledColor : kAvailableColor);
    }
  }
}

void SampleGateView::Tap(Pad *pad) {
  if (_delegate) {
    return;
  }
  PadIndex pad_index = pad->GetPadIndex();
  if (_pad_index_mapping.find(pad_index) == _pad_index_mapping.end()) {
    return;
  }
  _microstep_ticks ^= _pad_index_mapping[pad_index];
  _delegate->Tap(_microstep_ticks);
}

void SampleGateView::Release(Pad *pad) {
  // no-op.
}
