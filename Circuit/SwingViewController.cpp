//
//  SwingViewController.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/28/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "SwingViewController.hpp"


#include <vector>

#include "CircuitController.hpp"
#include "Pad.hpp"
#include "CircuitView.hpp"
#include "DigitView.hpp"

SwingViewController::SwingViewController(CircuitController *parent) : ScreenController(parent) {
  std::vector<Pad *> pads = GetView()->GetRegularPads(0, kRegularPadCount);
  _digit_view = new DigitView(pads);
  for (Knob *knob : GetView()->GetKnobs()) {
    knob->SetDelegate(this);
  }
}
SwingViewController::~SwingViewController() {
  for (Knob *knob : GetView()->GetKnobs()) {
    knob->SetDelegate(nullptr);
  }
  delete _digit_view;
}

void SwingViewController::Update() {
  Swing swing = GetParent()->GetSwing();
  _digit_view->SetNumber(swing);
  for (Knob *knob : GetView()->GetKnobs()) {
    KnobIndex index = knob->GetKnobIndex();
    knob->SetCC(index == 0 ? swing :0);
  }
}


void SwingViewController::Change(Knob *knob, const CC &cc) {
  KnobIndex index = knob->GetKnobIndex();
  if (index != 0) {
    return;
  }
  Swing swing = cc;
  if (swing < 20) {
    swing = 20;
  } else if (swing > 70) {
    swing = 70;
  }
  GetParent()->SetSwing(swing);
}

void SwingViewController::UpdateEditingMode() {
  // no-op
}

void SwingViewController::UpdateRunningMode() {
  // no-op
}
