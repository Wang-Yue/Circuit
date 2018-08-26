//
//  NudgeViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/25/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef NudgeViewController_hpp
#define NudgeViewController_hpp

#include <map>
#include <list>
#include "Pattern.hpp"

#include "NudgeView.hpp"

template <typename AtomClass = Atom>
class NudgeViewController : public NudgeViewDelegate {
  
public:
  NudgeViewController(const std::vector<Pad *> &pads) : _pattern(nullptr) {
    _view = new NudgeView(pads, this);
  }
  
  virtual ~NudgeViewController() {
    delete _view;
  }
  
  void SetPattern(Pattern<AtomClass> *pattern) {
    if (_pattern == pattern) {
      return;
    }
    _pattern = pattern;
  }
  
  virtual void NudgeLeft() override{
    if (_pattern) {
      _pattern->NudgeLeft();
    }
  }
  
  virtual void NudgeRight() override {
    if (_pattern) {
      _pattern->NudgeRight();
    }
  }
  
  void Update() {
    // no-op.
  }
  
private:
  NudgeView *_view;
  Pattern<AtomClass> *_pattern;
};

#endif /* NudgeViewController_hpp */
