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

#include "GateView.hpp"

template <typename AtomClass = Atom>
class GateViewController : public GateViewDelegate {
  
public:
  GateViewController(const std::vector<Pad *> &pads) :
  _current_step(nullptr), _current_editing_step(nullptr) {
    _view = new GateView(pads, this);
  }
  
  
  virtual ~GateViewController() {
    delete _view;
  }
  
  void SetCurrentStep(Step<AtomClass> *current_step) {
    if (current_step == _current_step) {
      return;
    }
    _current_step = current_step;
  }
  
  void SetCurrentEditingStep(Step<AtomClass> *current_editing_step) {
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
  
  void TickStep() {
    Gate gate = 0;
    if (_current_editing_step) {
      gate = _current_editing_step->GetGate();
    } else if (_current_step) {
      gate = _current_step->GetGate();
    }
    _view->SetGate(gate);
  }
  
private:
  GateView *_view;
  Step<AtomClass> *_current_step;
  Step<AtomClass> *_current_editing_step;
};

#endif /* GateViewController_hpp */
