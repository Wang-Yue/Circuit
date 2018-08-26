//
//  SampleGateViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/26/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef SampleGateViewController_hpp
#define SampleGateViewController_hpp

#include "SampleGateView.hpp"

template <typename AtomClass> class Step;
class Sample;

class SampleGateViewController : public SampleGateViewDelegate {
public:
  SampleGateViewController(const std::vector<Pad *> &pads);
  virtual ~SampleGateViewController();
  void SetCurrentStep(Step<Sample> *current_step);
  void SetCurrentEditingStep(Step<Sample> *current_editing_step);
  void Update();
  // SampleGateViewDelegate.
  virtual void Tap(const Microstep &microstep_ticks) override;
private:
  SampleGateView *_view;
  Step<Sample> *_current_step;
  Step<Sample> *_current_editing_step;
};
#endif /* SampleGateViewController_hpp */
