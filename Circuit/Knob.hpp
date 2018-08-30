//
//  Knob.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/29/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef Knob_hpp
#define Knob_hpp

#include "Color.hpp"
#include "UIDefs.hpp"

class Knob;

class KnobDelegate {
public:
  virtual void Change(Knob *, const CC &cc) = 0;
};

class Knob {
public:
  Knob(const KnobIndex &knob_index);
  KnobIndex GetKnobIndex() const;
  CC GetCC() const;
  void SetCC(const CC &cc);
  void SetDelegate(KnobDelegate *delegate);
  void Change(const CC &cc);
private:
  const KnobIndex _knob_index;
  CC _cc;
  KnobDelegate *_delegate;
};

#endif /* Knob_hpp */
