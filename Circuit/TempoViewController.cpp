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

}
TempoViewController::~TempoViewController() {
  delete _digit_view;
}

void TempoViewController::Update() {
  BPM bpm = GetParent()->GetBPM();
  _digit_view->SetNumber(bpm);
}

void TempoViewController::UpdateEditingMode() {
  // no-op
}

void TempoViewController::UpdateRunningMode() {
  // no-op
}
