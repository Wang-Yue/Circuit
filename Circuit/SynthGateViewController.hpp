//
//  GateViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/24/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef GateViewController_hpp
#define GateViewController_hpp

#include <vector>

#include "SynthGateView.hpp"

template <typename AtomClass> class Step;
class Synth;

class SynthGateViewController : public SynthGateViewDelegate {
public:
  SynthGateViewController(const std::vector<Pad *> &pads);
  virtual ~SynthGateViewController();
  void SetCurrentStep(Step<Synth> *current_step);
  void SetCurrentEditingStep(Step<Synth> *current_editing_step);
  void Update();
  // SynthGateViewDelegate.
  virtual void Tap(const Gate &gate) override;
private:
  SynthGateView *_view;
  Step<Synth> *_current_step;
  Step<Synth> *_current_editing_step;
};

#endif /* GateViewController_hpp */
