//
//  GateViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/24/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef GateViewController_hpp
#define GateViewController_hpp

#include "KeyboardView.hpp"
#include <map>
#include <list>
#include "Pattern.hpp"
#include "Step.hpp"

#include "SynthGateView.hpp"

class SynthGateViewController : public SynthGateViewDelegate {
  
public:
  SynthGateViewController(const std::vector<Pad *> &pads) :
  _current_step(nullptr), _current_editing_step(nullptr) {
    _view = new SynthGateView(pads, this);
  }
  
  
  virtual ~SynthGateViewController() {
    delete _view;
  }
  
  void SetCurrentStep(Step<Synth> *current_step) {
    if (current_step == _current_step) {
      return;
    }
    _current_step = current_step;
  }
  
  void SetCurrentEditingStep(Step<Synth> *current_editing_step) {
    if (_current_editing_step == current_editing_step) {
      return;
    }
    _current_editing_step = current_editing_step;
  }

  
  virtual void Tap(const Gate &gate) override {
    if (_current_editing_step) {
      _current_editing_step->SetGate(gate);
    }
  }
  
  void Update() {
    Gate gate = 0;
    if (_current_editing_step) {
      gate = _current_editing_step->GetGate();
    } else if (_current_step) {
      gate = _current_step->GetGate();
    }
    _view->SetGate(gate);
  }
  
private:
  SynthGateView *_view;
  Step<Synth> *_current_step;
  Step<Synth> *_current_editing_step;
};

#endif /* GateViewController_hpp */
