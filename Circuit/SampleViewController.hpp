//
//  SampleViewController.hpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef SampleViewController_hpp
#define SampleViewController_hpp

#include "ScreenController.hpp"

#include "TypeDefs.hpp"
#include <vector>
#include <map>
#include "PatternViewController.hpp"

class CircuitController;
class SampleGateViewController;
class Sample;
template <typename AtomClass> class VelocityViewController;
template <typename AtomClass> class LengthViewController;
template <typename AtomClass> class NudgeViewController;


class SampleViewController :
public ScreenController,
public PatternViewControllerDelegate<Sample> {
public:
  SampleViewController(CircuitController *parent, const ChannelIndex &channel);
  virtual ~SampleViewController();
  // ScreenController.
  virtual void Update() override;
  virtual void UpdateRunningMode() override;
  virtual void UpdateEditingMode() override;
  // PatternViewControllerDelegate.
  virtual void SelectStep(Step<Sample> *step, const StepIndex &selected_index) override;
  virtual void ReleaseStep(const StepIndex &selected_index) override;
private:
  void KillAllControllers();
  const ChannelIndex _channel_index;
  const ChannelIndex _companion_channel_index;
  PatternViewController<Sample> *_pattern_view_controller;
  PatternViewController<Sample> *_companion_pattern_view_controller;
  SampleGateViewController *_gate_view_controller;
  VelocityViewController<Sample> *_velocity_view_controller;
  LengthViewController<Sample> *_length_view_controller;
  NudgeViewController<Sample> *_nudge_view_controller;
  StepIndex _editing_step_index;
  Step<Sample> *_editing_step;
};
#endif /* SampleViewController_hpp */
