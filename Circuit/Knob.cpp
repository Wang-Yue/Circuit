//
//  Knob.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/29/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "Knob.hpp"


Knob::Knob(const KnobIndex &knob_index): _knob_index(knob_index), _cc(0), _delegate(nullptr){
}


KnobIndex Knob::GetKnobIndex() const {
  return _knob_index;
}

CC Knob::GetCC() const {
  return _cc;
}

void Knob::SetCC(const CC &cc) {
  _cc = cc;
}

void Knob::SetDelegate(KnobDelegate *delegate) {
  if (_delegate != delegate) {
    _delegate = delegate;
  }
}

void Knob::Change(const CC &cc) {
  if (_delegate) {
    _delegate->Change(this, cc);
  }
}

