//
//  SynthViewController.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "SynthViewController.hpp"

#include "SampleViewController.hpp"
#include "CircuitView.hpp"
#include "PatternViewController.hpp"
#include "KeyboardViewController.hpp"
#include "SessionRunner.hpp"
#include "ChannelRunner.hpp"
#include "PatternChainRunner.hpp"
#include "Session.hpp"
#include "KeyboardViewController.hpp"
#include "SynthGateViewController.hpp"
#include "VelocityViewController.hpp"
#include "LengthViewController.hpp"
#include "NudgeViewController.hpp"

SynthViewController::SynthViewController(CircuitController *parent,
                                         const ChannelIndex &channel) :
ScreenController(parent),
_channel_index(channel),
_pattern_controller(nullptr),
_keyboard_controller(nullptr),
_gate_view_controller(nullptr),
_velocity_view_controller(nullptr),
_length_view_controller(nullptr),
_nudge_view_controller(nullptr),
_editing_step(nullptr) {
  UpdateEditingMode();
}

SynthViewController::~SynthViewController() {
  KillAllControllers();
}

void SynthViewController::KillAllControllers() {
  if (_pattern_controller) {
    delete _pattern_controller;
    _pattern_controller = nullptr;
  }
  if (_keyboard_controller) {
    delete _keyboard_controller;
    _keyboard_controller = nullptr;
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

void SynthViewController::UpdateEditingMode() {
  KillAllControllers();

  CircuitEditingMode mode = GetEditingMode();
  PadIndex pads_count = kRegularPadCount;

  Note base_note = GetCurrentSession()->GetBaseNote();
  Degree tonic_degree = GetCurrentSession()->GetTonicDegree();
  if (mode == CircuitEditExpandNoteMode) {
      std::vector<Pad *> keyboard_pads = GetView()->GetRegularPads(0, pads_count);
      _keyboard_controller = new KeyboardViewController(base_note, tonic_degree, keyboard_pads, this);
      // We don't need to create the pattern view so return early.
      return;
  }
  // Put the pattern pads to the last.
  pads_count -= kStepCapacity;
  std::vector<Pad *> pattern_pads = GetView()->GetRegularPads(pads_count, kStepCapacity);
  Pattern<Synth> *pattern = GetCurrentSynthPattern(_channel_index);
  _pattern_controller = new PatternViewController<Synth>(pattern_pads, pattern, this);

  std::vector<Pad *> remaining_pads = GetView()->GetRegularPads(0, pads_count);

  if (mode == CircuitEditNoteMode) {
    _keyboard_controller = new KeyboardViewController(base_note, tonic_degree, remaining_pads, this);
  } else if (mode == CircuitEditGateMode) {
    _gate_view_controller = new SynthGateViewController(remaining_pads);
  } else if (mode == CircuitEditVelocityMode) {
    _velocity_view_controller = new VelocityViewController<Synth>(remaining_pads);
  } else if (mode == CircuitEditNudgeMode) {
    _nudge_view_controller = new NudgeViewController<Synth>(remaining_pads);
  } else if (mode == CircuitEditLengthMode) {
    _length_view_controller = new LengthViewController<Synth>(remaining_pads);
  }
}

void SynthViewController::UpdateRunningMode() {
  _editing_step = nullptr;
}

void SynthViewController::Update() {
  Pattern<Synth> *pattern = GetCurrentSynthPattern(_channel_index);
  PatternChainRunner<Synth> * pattern_chain_runner = GetSynthPatternChainRunner(_channel_index);

  Step<Synth> *current_step = pattern_chain_runner->GetStep();
  StepIndex current_step_index = pattern_chain_runner->GetStepCounter();
  if (_pattern_controller) {
    _pattern_controller->SetPattern(pattern);
    _pattern_controller->SetSelectedStep(_editing_step, _editing_step_index);
    _pattern_controller->SetCurserStep(current_step, current_step_index);
    _pattern_controller->Update();
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
  
  ChannelRunner<Synth> *channel_runner = GetSynthChannelRunner(_channel_index);
  
  if (_keyboard_controller) {
    // Behave the same in playing, record mode.
    _keyboard_controller->SetEditingStep(_editing_step);
    std::list<Note> notes;
    
    // In stop mode, we don't show the playing notes from channel. We only show user's tapping notes.
    if (!IsStopped()) {
      notes = channel_runner->GetPlayingNotes();
    }
    for (std::pair<Synth * const, Note> &p : _active_atoms) {
      Note note = p.second;
      notes.push_back(note);
    }

    _keyboard_controller->SetPlayingNotes(notes);
    _keyboard_controller->Update();
  }
  // TODO: revisit this approach.
  for (std::pair<Synth * const, Note> &p : _active_atoms) {
    Synth *s = p.first;
    Gate gate = s->GetGate();
    gate += kMicrosteps;
    if (gate >= kStepCapacity * kMicrosteps) {
      gate = kStepCapacity * kMicrosteps;
    }
    s->SetGate(gate);
  }
  
}

void SynthViewController::SelectStep(Step<Synth> *step, const StepIndex &selected_index) {
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

void SynthViewController::ReleaseStep(const StepIndex &selected_index) {
  // In playing and redording mode, we reset the editing step, as edit only happens when a step is
  // long tapped.
  if (!IsStopped()) {
    _editing_step = nullptr;
    return;
  }
}

void SynthViewController::TapNote(const Note &note) {
  if (_editing_step) {
    if (!_editing_step->RemoveNote(note)) {
      _editing_step->AddNote(note);
    }
  } else {
    PatternChainRunner<Synth> * pattern_chain_runner = GetSynthPatternChainRunner(_channel_index);
    Step<Synth> *step = pattern_chain_runner->GetStep();
    Synth *s = step->AddNote(note);
    _active_atoms[s] = note;
  }
}

void SynthViewController::ReleaseNote(const Note &note) {
  if (_editing_step) {
    // no-op.
  } else {
    for (std::map<Synth *, Note>::iterator it = _active_atoms.begin();
         it != _active_atoms.end();
         /* empty */) {
      if (NoteIdentical(it->second, note)) {
      it = _active_atoms.erase(it);
      } else {
        ++it;
      }
    }
  }
}

