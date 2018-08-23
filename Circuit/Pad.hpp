//
//  Pad.hpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef Pad_hpp
#define Pad_hpp

#include "Color.hpp"
#include "TypeDefs.hpp"

class Pad;

class PadDelegate {
public:
  virtual void Tap(Pad *) = 0;
  virtual void Release(Pad *) = 0;
};

class Pad {
public:
  Pad(const PadIndex &pad_index) : _pad_index(pad_index), _delegate(nullptr) {
  }
  PadIndex GetPadIndex() const {
    return _pad_index;
  }
  Color GetColor()  const {
    return _color;
  }
  void SetColor(const Color &color) {
    // TODO: optimize by adding IsEqual to color
//    if (color == _color) {
//      return;
//    }
    _color = color;
  }
  
  void SetDelegate(PadDelegate *delegate) {
    _delegate = delegate;
  }
  void Tap() {
    if (_delegate) {
      _delegate->Tap(this);
    }
  }
  void Release() {
    if (_delegate) {
      _delegate->Release(this);
    }
  }
private:
  const PadIndex _pad_index;
  Color _color;
  PadDelegate *_delegate;
};

#endif /* Pad_hpp */
