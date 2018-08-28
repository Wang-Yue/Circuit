//
//  LengthView.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/24/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "LengthView.hpp"


static const Color kZeroLengthColor(0x00000000);
static const Color kFullLengthColor(0x0000ff00);

LengthView::LengthView(const std::vector<Pad *> &pads, LengthViewDelegate *delegate):
_pads(pads), _delegate(delegate) {
  assert(pads.size() == kStepCapacity);
  for (PadIndex i = 0; i < _pads.size(); ++i) {
    Pad *pad = _pads[i];
    _pad_index_mapping[pad->GetPadIndex()] = i;
    pad->SetDelegate(this);
  }
}

LengthView::~LengthView() {
  for (Pad *pad : _pads) {
    pad->SetDelegate(nullptr);
  }
}

void LengthView::SetLength(const StepIndex &length) {
  for (PadIndex i = 0; i < _pads.size(); ++i ){
    Pad *pad = _pads[i];
    if (i < length) {
      pad->SetColor(kFullLengthColor);
    } else {
      pad->SetColor(kZeroLengthColor);
    }
  }
}

void LengthView::Tap(Pad *pad) {
  PadIndex pad_index = pad->GetPadIndex();
  StepIndex length = _pad_index_mapping[pad_index] + 1;
  if (_delegate) {
    _delegate->Tap(length);
  }
}

void LengthView::Release(Pad *pad) {
  // no-op.
}
