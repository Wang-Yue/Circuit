//
//  SynthMicrostepDelayViewController.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/28/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "SynthMicrostepDelayViewController.hpp"


#include "Pattern.hpp"
#include "Step.hpp"

SynthMicrostepDelayViewController::SynthMicrostepDelayViewController(const std::vector<Pad *> &pads) :
_step(nullptr) {
  _view = new SynthMicrostepDelayView(pads, this);
}

SynthMicrostepDelayViewController::~SynthMicrostepDelayViewController() {
  delete _view;
}

void SynthMicrostepDelayViewController::SetStep(Step<Synth> *step) {
  if (step == _step) {
    return;
  }
  _step = step;
}



void SynthMicrostepDelayViewController::TapMicrostepDelay(const Microstep &delay) {
  if (_step) {
    _step->SetMicrostepDelay(delay);
  }
}

void SynthMicrostepDelayViewController::ToggleTie() {
  if (_step) {
    bool tie = _step->GetTie();
    _step->SetTie(!tie);
  }
}

void SynthMicrostepDelayViewController::Update() {
  if (_step) {
    _view->SetMicrostepDelay(_step->GetMicrostepDelay(), _step->GetTie());
  } else {
    _view->SetMicrostepDelay(0, false);
  }
}
