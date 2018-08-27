//
//  Pad.cpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "Pad.hpp"

Pad::Pad(const PadIndex &pad_index) : _pad_index(pad_index), _delegate(nullptr) {
}

PadIndex Pad::GetPadIndex() const {
  return _pad_index;
}

Color Pad::GetColor()  const {
  return _color;
}

void Pad::SetColor(const Color &color) {
  _color = color;
}

void Pad::SetDelegate(PadDelegate *delegate) {
  _delegate = delegate;
}

void Pad::Tap() {
  if (_delegate) {
    _delegate->Tap(this);
  }
}

void Pad::Release() {
  if (_delegate) {
    _delegate->Release(this);
  }
}
