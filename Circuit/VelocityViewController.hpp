//
//  VelocityViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/24/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef VelocityViewController_hpp
#define VelocityViewController_hpp

#include "KeyboardView.hpp"
#include <map>
#include <list>
#include "Pattern.hpp"
#include "Step.hpp"

#include "VelocityView.hpp"

template <typename AtomClass = Atom>
class VelocityViewController : public VelocityViewDelegate {
  
public:
  VelocityViewController(const std::vector<Pad *> &pads) :
  _current_step(nullptr), _current_editing_step(nullptr) {
    _view = new VelocityView(pads, this);
  }
  
  
  virtual ~VelocityViewController() {
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
  
  
  virtual void Tap(const Velocity &velocity) override {
    if (_current_editing_step) {
      _current_editing_step->SetVelocity(velocity);
    }
  }
  
  void Update() {
    Velocity velocity = 0;
    if (_current_editing_step) {
      velocity = _current_editing_step->GetVelocity();
    } else if (_current_step) {
      velocity = _current_step->GetVelocity();
    }
    _view->SetVelocity(velocity);
  }
  
private:
  VelocityView *_view;
  Step<AtomClass> *_current_step;
  Step<AtomClass> *_current_editing_step;
};

#endif /* VelocityViewController_hpp */
