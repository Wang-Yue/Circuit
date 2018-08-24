//
//  GateView.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/24/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef GateView_hpp
#define GateView_hpp

#include <vector>
#include <map>
#include "TypeDefs.hpp"
#include "Pad.hpp"
#include "Color.hpp"

class GateViewDelegate {
public:
  virtual void Tap(const Gate &gate) = 0;
};

class GateView : public PadDelegate {
public:
  GateView(const std::vector<Pad *> &pads, GateViewDelegate *delegate);
  void SetGate(const Gate &gate);
  virtual void Tap(Pad *pad) override;
  virtual void Release(Pad *pad) override;
private:
  std::vector<Pad *> _pads;
  Gate _previous_gate;
  // pad's own PadIndex -> index in _pads vector.
  std::map<PadIndex, PadIndex> _pad_index_mapping;
  GateViewDelegate *_delegate;
};

#endif /* GateView_hpp */
