//
//  SynthMicrostepDelayViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/28/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef SynthMicrostepDelayViewController_hpp
#define SynthMicrostepDelayViewController_hpp

#include <vector>

#include "SynthMicrostepDelayView.hpp"

template <typename AtomClass> class Step;
class Synth;

class SynthMicrostepDelayViewController : public SynthMicrostepDelayViewDelegate {
public:
  SynthMicrostepDelayViewController(const std::vector<Pad *> &pads);
  virtual ~SynthMicrostepDelayViewController();
  void SetStep(Step<Synth> *_step);
  void Update();
  // SynthMicrostepDelayViewDelegate.
  virtual void TapMicrostepDelay(const Microstep &delay) override;
  virtual void ToggleTie() override;
private:
  SynthMicrostepDelayView *_view;
  Step<Synth> *_step;
};

#endif /* SynthMicrostepDelayViewController_hpp */
