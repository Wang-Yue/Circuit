//
//  TempoViewController.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/28/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "TempoViewController.hpp"

#include <vector>

#include "CircuitController.hpp"
#include "Pad.hpp"
#include "CircuitView.hpp"
#include "DigitView.hpp"

TempoViewController::TempoViewController(CircuitController *parent) : ScreenController(parent) {
  std::vector<Pad *> pads = GetView()->GetRegularPads(0, kRegularPadCount);
  _digit_view = new DigitView(pads);
  for (Knob *knob : GetView()->GetKnobs()) {
    knob->SetDelegate(this);
  }
}
TempoViewController::~TempoViewController() {
  for (Knob *knob : GetView()->GetKnobs()) {
    knob->SetDelegate(nullptr);
  }
  delete _digit_view;
}

void TempoViewController::Update() {
  BPM bpm = GetParent()->GetBPM();
  _digit_view->SetNumber(bpm);
  for (Knob *knob : GetView()->GetKnobs()) {
    KnobIndex index = knob->GetKnobIndex();
    knob->SetCC(index == 0 ? (bpm - 1) / 2 : 0);
  }
}

void TempoViewController::Change(Knob *knob, const CC &cc) {
  KnobIndex index = knob->GetKnobIndex();
  if (index != 0) {
    return;
  }
  BPM bpm = cc * 2 + 1;
  if (bpm < 20) {
    bpm = 20;
  } else if (bpm > 250) {
    bpm = 250;
  }
  GetParent()->SetBPM(bpm);
}

void TempoViewController::UpdateEditingMode() {
  // no-op
}

void TempoViewController::UpdateRunningMode() {
  // no-op
}
