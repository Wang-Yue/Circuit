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
#include "UIDefs.hpp"

class Pad;

class PadDelegate {
public:
  virtual void Tap(Pad *) = 0;
  virtual void Release(Pad *) = 0;
};

class Pad {
public:
  Pad(const PadIndex &pad_index);
  PadIndex GetPadIndex() const;
  Color GetColor()  const;
  void SetColor(const Color &color);
  void SetDelegate(PadDelegate *delegate);
  void Tap();
  void Release();
private:
  const PadIndex _pad_index;
  Color _color;
  PadDelegate *_delegate;
};

#endif /* Pad_hpp */
