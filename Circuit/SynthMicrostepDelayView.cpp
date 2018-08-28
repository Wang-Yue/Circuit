//
//  SynthMicrostepDelayView.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/28/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "SynthMicrostepDelayView.hpp"


#include "Color.hpp"

static const Color kDisabledColor(0x00000000);
static const Color kEnabledColor(0x000000ff);
static const Color kAvaliableColor(0x0000005f);

SynthMicrostepDelayView::SynthMicrostepDelayView(const std::vector<Pad *> &pads,
                                                 SynthMicrostepDelayViewDelegate *delegate):
_pads(pads), _delegate(delegate) {
  assert(pads.size() == kStepCapacity);
  for (PadIndex i = 0; i < _pads.size(); ++i) {
    Pad *pad = _pads[i];
    if (i < kMicrosteps) {
      _pad_microstep_mapping[pad->GetPadIndex()] = i;
      pad->SetDelegate(this);
    } else if (i == kPadsInRow - 1) {
      _tie_pad_index = pad->GetPadIndex();
      pad->SetDelegate(this);
    } else {
      pad->SetDelegate(nullptr);
    }
  }
}

SynthMicrostepDelayView::~SynthMicrostepDelayView() {
  for (Pad *pad : _pads) {
    pad->SetDelegate(nullptr);
  }
}

void SynthMicrostepDelayView::SetMicrostepDelay(const Microstep &delay, const bool &tie) {
  for (PadIndex i = 0; i < _pads.size(); ++i ){
    Pad *pad = _pads[i];
    if (i < kMicrosteps) {
      if (i == delay) {
        pad->SetColor(kEnabledColor);
      } else {
        pad->SetColor(kAvaliableColor);
      }
    } else if (i == _tie_pad_index) {
      pad->SetColor(tie ? kEnabledColor : kAvaliableColor);
    } else {
      pad->SetColor(kDisabledColor);
    }
  }
}

void SynthMicrostepDelayView::Tap(Pad *pad) {
  if (!_delegate) {
    return;
  }
  PadIndex pad_index = pad->GetPadIndex();
  if (pad_index == _tie_pad_index) {
    _delegate->ToggleTie();
  } else {
    Microstep microstep = _pad_microstep_mapping[pad_index];
    _delegate->TapMicrostepDelay(microstep);
  }
}

void SynthMicrostepDelayView::Release(Pad *pad) {
  // no-op.
}
