//
//  SynthGateViewController.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/26/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "SynthGateViewController.hpp"

#include "Pattern.hpp"
#include "Step.hpp"

SynthGateViewController::SynthGateViewController(const std::vector<Pad *> &pads) :
_current_step(nullptr), _current_editing_step(nullptr) {
  _view = new SynthGateView(pads, this);
}

SynthGateViewController::~SynthGateViewController() {
  delete _view;
}

void SynthGateViewController::SetCurrentStep(Step<Synth> *current_step) {
  if (current_step == _current_step) {
    return;
  }
  _current_step = current_step;
}

void SynthGateViewController::SetCurrentEditingStep(Step<Synth> *current_editing_step) {
  if (_current_editing_step == current_editing_step) {
    return;
  }
  _current_editing_step = current_editing_step;
}


void SynthGateViewController::Tap(const Gate &gate)  {
  if (_current_editing_step) {
    _current_editing_step->SetGate(gate);
  }
}

void SynthGateViewController::Update() {
  Gate gate = 0;
  if (_current_editing_step) {
    gate = _current_editing_step->GetGate();
  } else if (_current_step) {
    gate = _current_step->GetGate();
  }
  _view->SetGate(gate);
}
