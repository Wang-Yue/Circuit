//
//  VelocityView.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/24/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef VelocityView_hpp
#define VelocityView_hpp


#include <vector>
#include <map>
#include "TypeDefs.hpp"
#include "Pad.hpp"
#include "Color.hpp"

class VelocityViewDelegate {
public:
  virtual void Tap(const Velocity &velocity) = 0;
};

class VelocityView : public PadDelegate {
public:
  VelocityView(const std::vector<Pad *> &pads, VelocityViewDelegate *delegate);
  void SetVelocity(const Velocity &Velocity);
  
  virtual void Tap(Pad *pad) override;
  
  virtual void Release(Pad *pad) override;
private:
  std::vector<Pad *> _pads;
  Velocity _previous_velocity;
  // pad's own PadIndex -> index in _pads vector.
  std::map<PadIndex, PadIndex> _pad_index_mapping;
  VelocityViewDelegate *_delegate;
};

#endif /* VelocityView_hpp */
