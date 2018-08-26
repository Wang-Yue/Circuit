//
//  NudgeView.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/25/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "NudgeView.hpp"


static const Color kUnusedColor(0x00000000);
static const Color kNudgeButtonColor(0x00007f00);

NudgeView::NudgeView(const std::vector<Pad *> &pads, NudgeViewDelegate *delegate):
_delegate(delegate) {
  assert(pads.size() == kStepCapacity);
  assert(pads.size() >= 2);
  _nudge_left_pad = pads[0];
  _nudge_left_pad->SetColor(kNudgeButtonColor);
  _nudge_left_pad->SetDelegate(this);
  _nudge_right_pad = pads[1];
  _nudge_right_pad->SetDelegate(this);
  _nudge_right_pad->SetColor(kNudgeButtonColor);
  for (PadIndex i = 2; i < pads.size(); ++i) {
    pads[i]->SetColor(kUnusedColor);
    pads[i]->SetDelegate(nullptr);
  }
}

NudgeView::~NudgeView() {
  _nudge_left_pad->SetDelegate(nullptr);
  _nudge_right_pad->SetDelegate(nullptr);
}

void NudgeView::Tap(Pad *pad) {
  if (_delegate) {
    if (pad == _nudge_right_pad) {
      _delegate->NudgeRight();
    } else if (pad == _nudge_left_pad) {
      _delegate->NudgeLeft();
    } else {
      throw std::runtime_error("Got message from unknown pad.");
    }
  }
}

void NudgeView::Release(Pad *pad) {
  // no-op.
}
