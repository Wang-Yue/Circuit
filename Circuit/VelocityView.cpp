//
//  VelocityView.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/24/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "VelocityView.hpp"
#include "UIDefs.hpp"

static const Color kZeroVelocityColor(0x00000000);
static const ColorCode kFullVelocityColorCode = 0x000000ff;
static const Color kFullVelocityColor(kFullVelocityColorCode);
static const uint8_t kVelocityIncrementPerPad = 8;

VelocityView::VelocityView(const std::vector<Pad *> &pads, VelocityViewDelegate *delegate):
_pads(pads), _delegate(delegate) {
  assert(pads.size() == kDiscreteVelocityValues / kVelocityIncrementPerPad);
  for (PadIndex i = 0; i < _pads.size(); ++i) {
    Pad *pad = _pads[i];
    _pad_index_mapping[pad->GetPadIndex()] = i;
    pad->SetDelegate(this);
  }
}

VelocityView::~VelocityView() {
  for (Pad *pad : _pads) {
    pad->SetDelegate(nullptr);
  }
}

void VelocityView::SetVelocity(const Velocity &velocity) {
  _previous_velocity = velocity;
  for (PadIndex i = 0; i < _pads.size(); ++i ){
    Pad *pad = _pads[i];
    PadIndex max_pad = velocity / kVelocityIncrementPerPad;
    if (i < max_pad) {
      pad->SetColor(kFullVelocityColor);
    } else if (i == max_pad) {
      static const ColorCode increment_color_per_velocity_value =
      (kFullVelocityColorCode + 1) / kVelocityIncrementPerPad;
      ColorCode code = increment_color_per_velocity_value * (velocity % kVelocityIncrementPerPad + 1) - 1;
      pad->SetColor(Color(code));
    } else {
      pad->SetColor(kZeroVelocityColor);
    }
  }
}

void VelocityView::Tap(Pad *pad) {
  PadIndex pad_index = pad->GetPadIndex();
  PadIndex previous_pad = _previous_velocity / kVelocityIncrementPerPad;
  if (pad_index != previous_pad) {
    _previous_velocity = (1 + pad_index) * kVelocityIncrementPerPad - 1;
  } else {
    --_previous_velocity;
    if (_previous_velocity % kVelocityIncrementPerPad == 0) {
      _previous_velocity += kVelocityIncrementPerPad - 1;
    }
  }
  if (_delegate) {
    _delegate->Tap(_previous_velocity);
  }
}

void VelocityView::Release(Pad *pad) {
  // no-op.
}
