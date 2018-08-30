//
//  ScaleSelectionView.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/26/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "ScaleSelectionView.hpp"


static const Color kEnabledColor(0x00ffff00);
static const Color kDisabledColor(0x000f0f00);

ScaleSelectionView::ScaleSelectionView(const std::vector<Pad *> &pads, ScaleSelectionViewDelegate *delegate) :
_pads(pads), _delegate(delegate){
  assert(pads.size() == ScaleUnknown);
  for (PadIndex i = 0; i < pads.size(); ++i) {
    Pad *pad = pads[i];
    PadIndex pad_index = pad->GetPadIndex();
    Scale scale = static_cast<Scale>(i);
    _scale_mapping[pad_index] = scale;
    pad->SetDelegate(this);
  }
}

ScaleSelectionView::~ScaleSelectionView() {
  for (Pad *pad : _pads) {
    pad->SetDelegate(nullptr);
  }
}

void ScaleSelectionView::SetScale(const Scale &scale) {
  for (Pad *pad : _pads) {
    Scale pad_scale = _scale_mapping[pad->GetPadIndex()];
    pad->SetColor(pad_scale == scale ? kEnabledColor: kDisabledColor);
  }
}

void ScaleSelectionView::Tap(Pad *pad) {
  if (_delegate) {
    Scale scale = _scale_mapping[pad->GetPadIndex()];
    _delegate->Tap(scale);
  }
}

void ScaleSelectionView::Release(Pad *pad)  {
  // no-op.
}
