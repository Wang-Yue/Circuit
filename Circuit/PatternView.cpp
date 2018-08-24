//
//  PatternView.cpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "PatternView.hpp"

static const Color kCurserColor(0x000000ff);
static const Color kAvailableKeyColor(0x0000ffff);
static const Color kDisabledKeyColor(0x00000000);
static const Color kGhostKeyColor(0x000000ff); // has note but disabled.
static const Color kEnabledKeyColor(0x00ffffff);
static const Color kSelectedKeyColor(0x00ffff00);


PatternView::PatternView(const std::map<StepIndex, Pad *> &step_pad_mapping, PatternViewDelegate *delegate):
_step_pad_mapping(step_pad_mapping), _delegate(delegate) {
  for (auto const& keyval : step_pad_mapping) {
    StepIndex step = keyval.first;
    Pad *pad = keyval.second;
    PadIndex pad_index = pad->GetPadIndex();
    _pad_step_mapping[pad_index] = step;
    pad->SetDelegate(this);
  }
}
PadIndex PatternView::PadCount() const {
  return _step_pad_mapping.size();
}
Pad *PatternView::GetPad(const StepIndex &step_index) {
  return _step_pad_mapping[step_index];
}

void PatternView::SetDisabled(const StepIndex &step_index) {
  Pad *pad = GetPad(step_index);
  pad->SetColor(kDisabledKeyColor);
}

void PatternView::SetEnabled(const StepIndex &step_index) {
  Pad *pad = GetPad(step_index);
  pad->SetColor(kEnabledKeyColor);
}

void PatternView::SetAvailable(const StepIndex &step_index) {
  Pad *pad = GetPad(step_index);
  pad->SetColor(kAvailableKeyColor);
}

void PatternView::SetGhost(const StepIndex &step_index) {
  Pad *pad = GetPad(step_index);
  pad->SetColor(kGhostKeyColor);
}

void PatternView::SetCurser(const StepIndex &step_index) {
  Pad *pad = GetPad(step_index);
  pad->SetColor(kCurserColor);
}

void PatternView::SetSelected(const StepIndex &step_index) {
  Pad *pad = GetPad(step_index);
  pad->SetColor(kSelectedKeyColor);
}

void PatternView::Tap(Pad *pad)  {
  PadIndex pad_index = pad->GetPadIndex();
  StepIndex step_index = _pad_step_mapping[pad_index];
  if (_delegate) {
    _delegate->Tap(step_index);
  }
}

void PatternView::Release(Pad *pad) {
  PadIndex pad_index = pad->GetPadIndex();
  StepIndex step_index = _pad_step_mapping[pad_index];
  if (_delegate) {
    _delegate->Release(step_index);
  }
}
