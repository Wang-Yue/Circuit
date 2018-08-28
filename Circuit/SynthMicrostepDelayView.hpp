//
//  SynthMicrostepDelayView.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/28/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef SynthMicrostepDelayView_hpp
#define SynthMicrostepDelayView_hpp


#include <vector>
#include <map>
#include "TypeDefs.hpp"
#include "Pad.hpp"

class SynthMicrostepDelayViewDelegate {
public:
  virtual void TapMicrostepDelay(const Microstep &delay) = 0;
  virtual void ToggleTie() = 0;
};

class SynthMicrostepDelayView : public PadDelegate {
public:
  SynthMicrostepDelayView(const std::vector<Pad *> &pads, SynthMicrostepDelayViewDelegate *delegate);
  ~SynthMicrostepDelayView();
  void SetMicrostepDelay(const Microstep &delay, const bool &tie);
  virtual void Tap(Pad *pad) override;
  virtual void Release(Pad *pad) override;
private:
  std::vector<Pad *> _pads;
  std::map<PadIndex, Microstep> _pad_microstep_mapping;
  PadIndex _tie_pad_index;
  SynthMicrostepDelayViewDelegate *_delegate;
};

#endif /* SynthMicrostepDelayView_hpp */
