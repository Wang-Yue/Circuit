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
#include "SynthPatternViewController.hpp"
#include "KeyboardViewController.hpp"
#include "SessionRunner.hpp"
#include "ChannelRunner.hpp"
#include "PatternChainRunner.hpp"
#include "Session.hpp"
#include "KeyboardViewController.hpp"
#include "SynthGateViewController.hpp"
#include "VelocityViewController.hpp"
#include "LengthViewController.hpp"

SynthViewController::SynthViewController(CircuitController *parent,
                                         const ChannelIndex &channel) :
ScreenController(parent),
_channel_index(channel),
_pattern_controller(nullptr),
_keyboard_controller(nullptr),
_gate_view_controller(nullptr),
_velocity_view_controller(nullptr),
_length_view_controller(nullptr) {
  UpdateEditingMode();
}

SynthViewController::~SynthViewController() {
  KillAllControllers();
}

void SynthViewController::KillAllControllers() {
  if (_pattern_controller) {
    delete _pattern_controller;
  }
  if (_keyboard_controller) {
    delete _keyboard_controller;
  }
  if (_gate_view_controller) {
    delete _gate_view_controller;
  }
  if (_velocity_view_controller) {
    delete _velocity_view_controller;
  }
  if (_length_view_controller) {
    delete _length_view_controller;
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
  _pattern_controller = new SynthPatternViewController(pattern_pads, pattern, this);

  std::vector<Pad *> remaining_pads = GetView()->GetRegularPads(0, pads_count);

  if (mode == CircuitEditNoteMode) {
    _keyboard_controller = new KeyboardViewController(base_note, tonic_degree, remaining_pads, this);
  } else if (mode == CircuitEditGateMode) {
    _gate_view_controller = new SynthGateViewController(remaining_pads);
  } else if (mode == CircuitEditVelocityMode) {
    _velocity_view_controller = new VelocityViewController<Synth>(remaining_pads);
  } else if (mode == CircuitEditNudgeMode) {
    // Unavailable for now
  } else if (mode == CircuitEditLengthMode) {
    _velocity_view_controller = new VelocityViewController<Synth>(remaining_pads);
  } else if (mode == CircuitEditLengthMode) {
    _length_view_controller = new LengthViewController<Synth>(remaining_pads);
  }
}

void SynthViewController::Update() {
  if (_pattern_controller) {
    _pattern_controller->SetPattern(GetCurrentSynthPattern(_channel_index));
    PatternChainRunner<Synth> * pattern_chain_runner = GetSynthPatternChainRunner(_channel_index);
    _pattern_controller->TickStep(pattern_chain_runner->GetStepCounter());
  }
  ChannelRunner<Synth> *channel_runner = GetSynthChannelRunner(_channel_index);
  std::list<Note> notes = channel_runner->GetPlayingNotes();
  
  if (_keyboard_controller) {
    for (std::pair<Synth * const, Note> &p : _active_atoms) {
      Note note = p.second;
      notes.push_back(note);
    }
    _keyboard_controller->Update();
    _keyboard_controller->ShowPlayingNotes(notes);
  }
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

void SynthViewController::SelectStep(Step<Synth> *step) {
  if (_keyboard_controller) {
    _keyboard_controller->SetEditingStep(step);
  }
}

void SynthViewController::ReleaseStep() {
  if (_keyboard_controller) {
    _keyboard_controller->SetEditingStep(nullptr);
  }
}

void SynthViewController::TapNote(const Note &note) {
  PatternChainRunner<Synth> * pattern_chain_runner = GetSynthPatternChainRunner(_channel_index);
  Step<Synth> *step = pattern_chain_runner->GetStep();
  Synth *s = step->AddNote(note);
  _active_atoms[s] = note;
}

void SynthViewController::ReleaseNote(const Note &note) {
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

