//
//  LengthViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/24/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef LengthViewController_hpp
#define LengthViewController_hpp

#include "KeyboardView.hpp"
#include <map>
#include <list>
#include "Pattern.hpp"
#include "Step.hpp"

#include "LengthView.hpp"

template <typename AtomClass = Atom>
class LengthViewController : public LengthViewDelegate {
  
public:
  LengthViewController(const std::vector<Pad *> &pads) : _pattern(nullptr) {
    _view = new LengthView(pads, this);
  }
  
  virtual ~LengthViewController() {
    delete _view;
  }
  
  void SetPattern(Pattern<AtomClass> *pattern) {
    if (_pattern == pattern) {
      return;
    }
    _pattern = pattern;
  }
  
  virtual void Tap(const StepIndex &length) override {
    if (_pattern) {
      _pattern->SetLength(length);
    }
  }
  
  void TickStep() {
    StepIndex length = 0;
    if (_pattern) {
      length = _pattern->GetLength();
    }
    _view->SetLength(length);
  }
  
private:
  LengthView *_view;
  Pattern<AtomClass> *_pattern;
};

#endif /* LengthViewController_hpp */
