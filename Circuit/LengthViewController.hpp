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
  LengthViewController(const std::vector<Pad *> &pads) :
  _pattern(nullptr), _companion_pattern(nullptr) {
    _view = new LengthView(pads, this);
  }
  
  virtual ~LengthViewController() {
    delete _view;
  }
  
  void SetPattern(Pattern<AtomClass> *pattern, Pattern<AtomClass> *companion_pattern) {
    if (_pattern == pattern && _companion_pattern == companion_pattern) {
      return;
    }
    _pattern = pattern;
    _companion_pattern = companion_pattern;
  }
  
  virtual void Tap(const StepIndex &length) override {
    if (_pattern) {
      _pattern->SetLength(length);
    }
    if (_companion_pattern) {
      _companion_pattern->SetLength(length);
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
  // For Drum 1 & 2; 3 & 4, once we call pattern-SetLength, we also change its companion pattern's
  // length.
  Pattern<AtomClass> *_companian_pattern;
};

#endif /* LengthViewController_hpp */
