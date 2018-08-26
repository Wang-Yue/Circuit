//
//  SampleViewController.cpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "SampleViewController.hpp"
#include "CircuitView.hpp"
#include "PatternViewController.hpp"
#include "SessionRunner.hpp"
#include "ChannelRunner.hpp"
#include "PatternChainRunner.hpp"
#include "SampleGateViewController.hpp"
#include "VelocityViewController.hpp"
#include "LengthViewController.hpp"
#include "NudgeViewController.hpp"
#include "Sample.hpp"

SampleViewController::SampleViewController(CircuitController *parent, const ChannelIndex &channel)
: ScreenController(parent),
_channel_index(channel),
_pattern_view_controller(nullptr),
_companion_pattern_view_controller(nullptr),
_gate_view_controller(nullptr),
_velocity_view_controller(nullptr),
_length_view_controller(nullptr),
_nudge_view_controller(nullptr),
_editing_step(nullptr) {
  UpdateEditingMode();
}

SampleViewController::~SampleViewController() {
  KillAllControllers();
}


void SampleViewController::KillAllControllers() {
  if (_pattern_view_controller) {
    delete _pattern_view_controller;
    _pattern_view_controller = nullptr;
  }
  if (_companion_pattern_view_controller) {
    delete _companion_pattern_view_controller;
    _companion_pattern_view_controller = nullptr;
  }
  if (_gate_view_controller) {
    delete _gate_view_controller;
    _gate_view_controller = nullptr;
  }
  if (_velocity_view_controller) {
    delete _velocity_view_controller;
    _velocity_view_controller = nullptr;
  }
  if (_length_view_controller) {
    delete _length_view_controller;
    _length_view_controller = nullptr;
  }
  if (_nudge_view_controller) {
    delete _nudge_view_controller;
    _nudge_view_controller = nullptr;
  }
}

void SampleViewController::UpdateEditingMode() {
  KillAllControllers();
  
  CircuitEditingMode mode = GetEditingMode();
  PadIndex pads_count = kRegularPadCount;
  
  if (mode == CircuitEditExpandNoteMode) {
    // TODO.
    // We don't need to create the pattern view so return early.
    return;
  }
  // Put the pattern pads to the last.
  pads_count -= kStepCapacity;
  std::vector<Pad *> pattern_pads = GetView()->GetRegularPads(pads_count, kStepCapacity);
  Pattern<Sample> *pattern = GetCurrentSamplePattern(_channel_index);
  _pattern_view_controller = new PatternViewController<Sample>(pattern_pads, pattern, this);
  
  std::vector<Pad *> remaining_pads = GetView()->GetRegularPads(0, pads_count);
  
  if (mode == CircuitEditNoteMode) {
    _companion_pattern_view_controller = new PatternViewController<Sample>(pattern_pads, pattern, this);
  } else if (mode == CircuitEditGateMode) {
    _gate_view_controller = new SampleGateViewController(remaining_pads);
  } else if (mode == CircuitEditVelocityMode) {
    _velocity_view_controller = new VelocityViewController<Sample>(remaining_pads);
  } else if (mode == CircuitEditNudgeMode) {
    _nudge_view_controller = new NudgeViewController<Sample>(remaining_pads);
  } else if (mode == CircuitEditLengthMode) {
    _length_view_controller = new LengthViewController<Sample>(remaining_pads);
  }
}

void SampleViewController::UpdateRunningMode() {
  _editing_step = nullptr;
}

void SampleViewController::Update() {
  Pattern<Sample> *pattern = GetCurrentSamplePattern(_channel_index);
  PatternChainRunner<Sample> * pattern_chain_runner = GetSamplePatternChainRunner(_channel_index);

  Step<Sample> *current_step = pattern_chain_runner->GetStep();
  StepIndex current_step_index = pattern_chain_runner->GetStepCounter();
  if (_pattern_view_controller) {
    _pattern_view_controller->SetPattern(pattern);
    _pattern_view_controller->SetSelectedStep(_editing_step, _editing_step_index);
    _pattern_view_controller->SetCurserStep(current_step, current_step_index);
    _pattern_view_controller->Update();
  }
  
  if (_gate_view_controller) {
    // Behave the same in playing, record, stop mode.
    _gate_view_controller->SetCurrentStep(current_step);
    _gate_view_controller->SetCurrentEditingStep(_editing_step);
    _gate_view_controller->Update();
  }
  
  if (_velocity_view_controller) {
    // Behave the same in playing, record, stop mode.
    _velocity_view_controller->SetCurrentStep(current_step);
    _velocity_view_controller->SetCurrentEditingStep(_editing_step);
    _velocity_view_controller->Update();
  }

  if (_length_view_controller) {
    // Behave the same in playing, record, stop mode.
    _length_view_controller->SetPattern(pattern, nullptr);
    _length_view_controller->Update();
  }
  
  if (_nudge_view_controller) {
    // Behave the same in playing, record, stop mode.
    _nudge_view_controller->SetPattern(pattern);
    _nudge_view_controller->Update();
  }
}

void SampleViewController::SelectStep(Step<Sample> *step, const StepIndex &selected_index) {
  if (IsStopped()) {
    // Toggle the step
    if (_editing_step != step) {
      _editing_step = step;
      _editing_step_index = selected_index;
    } else {
      _editing_step = nullptr;
    }
  } else {
    // Always update to the last selected index.
    _editing_step = step;
    _editing_step_index = selected_index;
  }
}

void SampleViewController::ReleaseStep(const StepIndex &selected_index) {
  // In playing and redording mode, we reset the editing step, as edit only happens when a step is
  // long tapped.
  if (!IsStopped()) {
    _editing_step = nullptr;
    return;
  }
}
