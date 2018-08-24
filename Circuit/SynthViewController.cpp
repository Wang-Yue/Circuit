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
#include "SessionRunner.hpp"
#include "ChannelRunner.hpp"
#include "PatternChainRunner.hpp"
#include "Session.hpp"
#include "KeyboardViewController.hpp"


SynthViewController::SynthViewController(CircuitController *parent,
                                         const ChannelIndex &channel,
                                         const bool &expand_keyboard)
: ScreenController(parent), _channel_index(channel), _synth_controller(nullptr), _keyboard_controller(nullptr) {
  PadIndex keyboard_pads_count = kPadCount;
  if (!expand_keyboard) {
    keyboard_pads_count -= kStepCapacity;
    std::vector<Pad *> pattern_pads = GetView()->GetPads(keyboard_pads_count, kStepCapacity);
    Pattern<Synth> *pattern = GetCurrentSynthPattern(channel);
    _synth_controller = new SynthPatternViewController(pattern_pads, pattern, this);
  }
  std::vector<Pad *> keyboard_pads = GetView()->GetPads(0, keyboard_pads_count);
  Note base_note = GetCurrentSession()->GetBaseNote();
  Degree tonic_degree = GetCurrentSession()->GetTonicDegree();
  _keyboard_controller = new KeyboardViewController(base_note, tonic_degree, keyboard_pads, this);
}

SynthViewController::~SynthViewController() {
  if (_synth_controller) {
    delete _synth_controller;
  }
  if (_keyboard_controller) {
    delete _keyboard_controller;
  }
}
void SynthViewController::TickStep() {
  if (_synth_controller) {
    _synth_controller->SetPattern(GetCurrentSynthPattern(_channel_index));
    PatternChainRunner<Synth> * pattern_chain_runner = GetSynthPatternChainRunner(_channel_index);
    _synth_controller->TickStep(pattern_chain_runner->GetStepCounter());
  }
  ChannelRunner<Synth> *channel_runner = GetSynthChannelRunner(_channel_index);
  std::list<Note> notes = channel_runner->GetPlayingNotes();
  

  for (std::pair<Synth * const, Note> &p : _active_atoms) {
    Note note = p.second;
    notes.push_back(note);
  }
  _keyboard_controller->TickStep();
  _keyboard_controller->ShowPlayingNotes(notes);
  
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
  _keyboard_controller->SetEditingStep(step);
}

void SynthViewController::ReleaseStep() {
  _keyboard_controller->SetEditingStep(nullptr);
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
