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
#include "ChannelOutputFactory.hpp"
#include "ChannelOutputInterface.hpp"
#include "Sample.hpp"

SampleViewController::SampleViewController(CircuitController *parent, const ChannelIndex &channel)
: ScreenController(parent),
_channel_index(channel),
_companion_channel_index(channel % 2 ? channel - 1 : channel + 1),
_pattern_view_controller(nullptr),
_companion_pattern_view_controller(nullptr),
_gate_view_controller(nullptr),
_velocity_view_controller(nullptr),
_length_view_controller(nullptr),
_nudge_view_controller(nullptr),
_patch_selection_view_controller(nullptr),
_expanded_note_view_controller(nullptr),
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
  if (_patch_selection_view_controller) {
    delete _patch_selection_view_controller;
    _patch_selection_view_controller = nullptr;
  }
  if (_expanded_note_view_controller) {
    delete _expanded_note_view_controller;
    _expand_note_view_tapped_channel.clear();
    _expanded_note_view_controller = nullptr;
  }
}

void SampleViewController::UpdateEditingMode() {
  KillAllControllers();
  
  CircuitEditingMode mode = GetEditingMode();
  
  PadIndex pads_count = kRegularPadCount;

  if (mode == CircuitEditExpandNoteMode) {
    ChannelIndex channel_count = GetCurrentSession()->GetSampleChannelCount();
    std::vector<Pad *> patch_pads = GetView()->GetRegularPads(0, pads_count);
    _expanded_note_view_controller =
        new SampleExpandNoteViewController(patch_pads, this, channel_count);
    for (ChannelIndex i = 0; i < channel_count; ++i) {
      _expand_note_view_tapped_channel.push_back(false);
    }
    // We don't need to create the pattern view so return early.
    return;
  }
  
  if (mode == CircuitEditPatchMode) {
    std::vector<Pad *> patch_pads = GetView()->GetRegularPads(0, pads_count);
    _patch_selection_view_controller = new SamplePatchSelectionViewController(patch_pads, this);
    // We don't need to create the pattern view so return early.
    return;
  }
  
  // Put the pattern pads to the last.

  pads_count -= kStepCapacity;
  std::vector<Pad *> pattern_pads;
  std::vector<Pad *> remaining_pads;
  if (_channel_index % 2 == 0) {
    remaining_pads = GetView()->GetRegularPads(pads_count, kStepCapacity);
    pattern_pads= GetView()->GetRegularPads(0, pads_count);
  } else {
    remaining_pads = GetView()->GetRegularPads(0, kStepCapacity);
    pattern_pads= GetView()->GetRegularPads(pads_count, pads_count);
  }
  
  Pattern<Sample> *pattern = GetCurrentSamplePattern(_channel_index);
  _pattern_view_controller = new PatternViewController<Sample>(pattern_pads, pattern, this);
  
  
  if (mode == CircuitEditNoteMode) {
    Pattern<Sample> *companion_pattern = GetCurrentSamplePattern(_companion_channel_index);
    _companion_pattern_view_controller =
        new PatternViewController<Sample>(remaining_pads, companion_pattern, this);
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
  if (_expanded_note_view_controller) {
    std::vector<bool> highlight_channels;
    for (ChannelIndex channel_index = 0;
         channel_index < GetCurrentSession()->GetSampleChannelCount();
         ++channel_index) {
      PatternChainRunner<Sample> * runner = GetSamplePatternChainRunner(channel_index);
      Step<Sample> *current_step = runner->GetStep();
      if (current_step->GetAtoms().size() || _expand_note_view_tapped_channel[channel_index]) {
        highlight_channels.push_back(true);
      } else {
        highlight_channels.push_back(false);
      }
    }
    _expanded_note_view_controller->SetHightlightChannels(highlight_channels);
    _expanded_note_view_controller->Update();
    // Expanded note view occupies whole screen so we can return early.
    return;
  }

  Pattern<Sample> *pattern = GetCurrentSamplePattern(_channel_index);
  Pattern<Sample> *compainion_pattern = GetCurrentSamplePattern(_companion_channel_index);
  PatternChainRunner<Sample> * pattern_chain_runner = GetSamplePatternChainRunner(_channel_index);
  PatternChainRunner<Sample> * companion_pattern_chain_runner =
      GetSamplePatternChainRunner(_companion_channel_index);

  Step<Sample> *current_step = pattern_chain_runner->GetStep();
  StepIndex current_step_index = pattern_chain_runner->GetStepCounter();
  Step<Sample> *companion_step = companion_pattern_chain_runner->GetStep();
  StepIndex companion_step_index = companion_pattern_chain_runner->GetStepCounter();

  if (_pattern_view_controller) {
    _pattern_view_controller->SetPattern(pattern);
    _pattern_view_controller->SetSelectedStep(_editing_step, _editing_step_index);
    _pattern_view_controller->SetCurserStep(current_step, current_step_index);
    _pattern_view_controller->Update();
  }
  
  if (_companion_pattern_view_controller) {
    _companion_pattern_view_controller->SetPattern(compainion_pattern);
    _companion_pattern_view_controller->SetCurserStep(companion_step, companion_step_index);
    _companion_pattern_view_controller->Update();
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
    // In Pattern related mode (nudge and length), we don't allow editing step.
    _editing_step = nullptr;
    // Behave the same in playing, record, stop mode.
    _length_view_controller->SetPattern(pattern, compainion_pattern);
    _length_view_controller->Update();
  }
  
  if (_nudge_view_controller) {
    // In Pattern related mode (nudge and length), we don't allow editing step.
    _editing_step = nullptr;
    // Behave the same in playing, record, stop mode.
    _nudge_view_controller->SetPattern(pattern);
    _nudge_view_controller->Update();
  }
  
  if (_patch_selection_view_controller) {
    std::vector<Sample *> atoms = current_step->GetAtoms();
    SynthIndex index;
    if (atoms.size()) {
      index = atoms[0]->GetSampleIndex();
    } else {
      Channel<Sample> *channel = pattern->GetChannel();
      index = channel->GetSampleIndex();
    }
    _patch_selection_view_controller->SetSelectedAtomPatchIndex(true, index);
    _patch_selection_view_controller->Update();
  }
}

void SampleViewController::SelectStep(Step<Sample> *step, const StepIndex &selected_index) {
  CircuitEditingMode mode = GetEditingMode();

  // In note mode it just toggle the step on/off.
  if (mode == CircuitEditNoteMode) {
    if (step->GetAtoms().size()) {
      step->RemoveAllAtoms();
    } else {
      Sample * atom = new Sample();
      step->AddAtom(atom);
    }
    return;
  }
  
  // In other modes, it behaves similar to the synth view controller.
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
  CircuitEditingMode mode = GetEditingMode();
  
  // In note mode it's a no-op the step on/off.
  if (mode == CircuitEditNoteMode) {
    // no-op.
    return;
  }
  
  // In playing and redording mode, we reset the editing step, as edit only happens when a step is
  // long tapped.
  if (!IsStopped()) {
    _editing_step = nullptr;
    return;
  }
}


void SampleViewController::TapPatch(const SynthIndex &index) {
  if (IsStopped()) {
    Pattern<Sample> *pattern = GetCurrentSamplePattern(_channel_index);
    Channel<Sample> *channel = pattern->GetChannel();
    channel->SetSampleIndex(index);
  }
  // When holding shift, we don't make sound. We also don't record.
  if (IsHoldingShift()) {
    return;
  }
  // Make the sound.
  SignalSample(_channel_index, index, kDefaultVelocity);
}

void SampleViewController::ReleasePatch(const SynthIndex &index) {
  // no-op
}

void SampleViewController::NoteOn(const MIDINote &note, const Velocity &velocity) {
  SignalSample(_channel_index, note, velocity);
}

void SampleViewController::NoteOff(const MIDINote &note) {
  // no-op.
}

void SampleViewController::TapChannel(const ChannelIndex &channel_index) {
  _expand_note_view_tapped_channel[channel_index] = true;
  Pattern<Sample> *pattern = GetCurrentSamplePattern(channel_index);
  Channel<Sample> *channel = pattern->GetChannel();
  SignalSample(channel_index, channel->GetSampleIndex(), kDefaultVelocity);
}

void SampleViewController::ReleaseChannel(const ChannelIndex &channel_index) {
  _expand_note_view_tapped_channel[channel_index] = false;
}
void SampleViewController::SignalSample(const ChannelIndex channel,
                                        const SampleIndex &index,
                                        const Velocity &velocity) {
  SampleChannelOutputInterface *output =
      ChannelOutputFactory::GetInstance().GetSampleChannelOutput(channel);
  output->Play(velocity, index);
  // If in record mode, record to the current step.
  if (IsRecording()) {
    Sample * atom = new Sample(index);
    atom->SetVelocity(velocity);
    PatternChainRunner<Sample> * pattern_chain_runner = GetSamplePatternChainRunner(channel);
    Step<Sample> *current_step = pattern_chain_runner->GetStep();
    current_step->AddAtom(atom);
  }
}
