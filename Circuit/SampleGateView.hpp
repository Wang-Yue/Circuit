//
//  SampleGateView.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/26/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef SampleGateView_hpp
#define SampleGateView_hpp


#include <vector>
#include <map>
#include "TypeDefs.hpp"
#include "Pad.hpp"

class SampleGateViewDelegate {
public:
  virtual void Tap(const Microstep &microstep_ticks) = 0;
};

class SampleGateView : public PadDelegate {
public:
  SampleGateView(const std::vector<Pad *> &pads, SampleGateViewDelegate *delegate);
  ~SampleGateView();
  void SetMicrostepTicks(const Microstep &microstep_ticks);
  virtual void Tap(Pad *pad) override;
  virtual void Release(Pad *pad) override;
private:
  std::vector<Pad *> _pads;
  Microstep _microstep_ticks;
  // pad's own Microstep -> index in _pads vector.
  std::map<PadIndex, Microstep> _pad_index_mapping;
  SampleGateViewDelegate *_delegate;
};

#endif /* SampleGateView_hpp */
