//
//  SynthViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef SynthViewController_hpp
#define SynthViewController_hpp

#include "ScreenController.hpp"

#include "SynthPatternViewController.hpp"
#include "KeyboardViewController.hpp"

#include "TypeDefs.hpp"
#include <vector>
#include <set>

class CircuitController;

class SynthPatternViewController;
class SynthGateViewController;
class KeyboardViewController;
class Synth;
template <typename AtomClass> class VelocityViewController;
template <typename AtomClass> class LengthViewController;

class SynthViewController :
public ScreenController,
public SynthPatternViewControllerDelegate,
public KeyboardViewControllerDelegate {
  
public:
  SynthViewController(CircuitController *parent,
                      const ChannelIndex &channel);
  virtual ~SynthViewController();
  virtual void Update() override;
  virtual void SelectStep(Step<Synth> *step, const StepIndex &selected_index) override;
  virtual void ReleaseStep(const StepIndex &selected_index) override;
  virtual void TapNote(const Note &note) override;
  virtual void ReleaseNote(const Note &note) override;
  virtual void UpdateRunningMode() override;
  virtual void UpdateEditingMode() override;
private:
  void KillAllControllers();
  const ChannelIndex _channel_index;
  SynthPatternViewController *_pattern_controller;
  KeyboardViewController *_keyboard_controller;
  SynthGateViewController *_gate_view_controller;
  VelocityViewController<Synth> *_velocity_view_controller;
  LengthViewController<Synth> *_length_view_controller;
  std::map<Synth *, Note> _active_atoms;
  StepIndex _editing_step_index;
  Step<Synth> *_editing_step;
};

#endif /* SynthViewController_hpp */
