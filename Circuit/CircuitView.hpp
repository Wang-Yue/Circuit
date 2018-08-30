//
//  CircuitView.hpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef CircuitView_hpp
#define CircuitView_hpp


#include <vector>

#include "UIDefs.hpp"
#include "TypeDefs.hpp"

class Pad;
class Knob;

class CircuitView {
public:
  CircuitView();
  ~CircuitView();
  std::vector<Pad *> GetRegularPads(const PadIndex &start, const PadIndex &count);
  Pad *GetPad(const PadIndex &index);
  Pad *GetControlPad(const PadIndex &index);
  std::vector<Knob *> GetKnobs();
  Knob *GetKnob(const KnobIndex &index) const;
private:
  std::vector<Pad *> _pads;
  std::vector<Knob *> _knobs;
};
#endif /* CircuitView_hpp */
