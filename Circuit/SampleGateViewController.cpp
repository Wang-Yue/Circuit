//
//  SampleGateViewController.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/26/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "SampleGateViewController.hpp"

#include "SampleGateViewController.hpp"

#include "Pattern.hpp"
#include "Step.hpp"

SampleGateViewController::SampleGateViewController(const std::vector<Pad *> &pads) :
_current_step(nullptr), _current_editing_step(nullptr) {
  _view = new SampleGateView(pads, this);
}


SampleGateViewController::~SampleGateViewController() {
  delete _view;
}

void SampleGateViewController::SetCurrentStep(Step<Sample> *current_step) {
  if (current_step == _current_step) {
    return;
  }
  _current_step = current_step;
}

void SampleGateViewController::SetCurrentEditingStep(Step<Sample> *current_editing_step) {
  if (_current_editing_step == current_editing_step) {
    return;
  }
  _current_editing_step = current_editing_step;
}


void SampleGateViewController::ToggleMicrostepTicks(const Microstep &toggle) {
  if (_current_editing_step) {
    Microstep ticks = _current_editing_step->GetMicrostepTicks();
    ticks ^= toggle;
    _current_editing_step->SetMicrostepTicks(ticks);
  }
}

void SampleGateViewController::Update() {
  Microstep microstep_ticks = 0;
  if (_current_editing_step) {
    microstep_ticks = _current_editing_step->GetMicrostepTicks();
  } else if (_current_step) {
    microstep_ticks = _current_step->GetMicrostepTicks();
  }
  _view->SetMicrostepTicks(microstep_ticks);
}
