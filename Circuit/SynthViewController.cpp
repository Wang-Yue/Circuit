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
#include "SynthMicrostepDelayViewController.hpp"
#include "VelocityViewController.hpp"
#include "LengthViewController.hpp"
#include "NudgeViewController.hpp"
#include "ChannelOutputFactory.hpp"
#include "ChannelOutputInterface.hpp"
#include "CircuitController.hpp"

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
_patch_selection_view_controller(nullptr),
_microstep_delay_view_controller(nullptr),
_editing_step(nullptr) {
  _output = ChannelOutputFactory::GetInstance().GetSynthChannelOutput(channel);
  UpdateEditingMode();
  SetMidiDelegate(this);
  for (Knob *knob : GetView()->GetKnobs()) {
    knob->SetDelegate(this);
  }
}

SynthViewController::~SynthViewController() {
  KillAllControllers();
  SetMidiDelegate(nullptr);
  for (Knob *knob : GetView()->GetKnobs()) {
    knob->SetDelegate(nullptr);
  }
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
  if (_patch_selection_view_controller) {
    delete _patch_selection_view_controller;
    _patch_selection_view_controller = nullptr;
  }
  if (_microstep_delay_view_controller) {
    delete _microstep_delay_view_controller;
    _microstep_delay_view_controller = nullptr;
  }
}

void SynthViewController::ReleaseImpromptuNotes() {
  for (NoteEvent &event : _impromptu_notes) {
    _output->NoteOff(event.midi_note);
  }
  _impromptu_notes.clear();
}

void SynthViewController::UpdateEditingMode() {
  ReleaseImpromptuNotes();
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
  if (mode == CircuitEditPatchMode) {
    std::vector<Pad *> patch_pads = GetView()->GetRegularPads(0, pads_count);
    _patch_selection_view_controller = new SynthPatchSelectionViewController(patch_pads, this);
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
  } else if (mode == CircuitEditSynthMicrostepDelayMode) {
    _microstep_delay_view_controller = new SynthMicrostepDelayViewController(remaining_pads);
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
  if (_microstep_delay_view_controller) {
    _microstep_delay_view_controller->SetStep(_editing_step);
    _microstep_delay_view_controller->Update();
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
    _length_view_controller->SetPattern(pattern, nullptr);
    _length_view_controller->Update();
  }
  
  if (_nudge_view_controller) {
    // In Pattern related mode (nudge and length), we don't allow editing step.
    _editing_step = nullptr;
    // Behave the same in playing, record, stop mode.
    _nudge_view_controller->SetPattern(pattern);
    _nudge_view_controller->Update();
  }
  
  ChannelRunner<Synth> *channel_runner = GetSynthChannelRunner(_channel_index);
  
  if (_keyboard_controller) {
    // Behave the same in playing, record mode.
    _keyboard_controller->SetEditingStep(_editing_step);
    std::list<Note> notes;
    
    // No need to get channel runner note in stop mode.
    if (IsRecording() || IsPlaying()) {
      notes = channel_runner->GetPlayingNotes();
    }
    for (const NoteEvent &e : _impromptu_notes) {
      notes.push_back(e.note);
    }
    _keyboard_controller->SetPlayingNotes(notes);
    _keyboard_controller->Update();
  }

  if (_patch_selection_view_controller) {
    Channel<Synth> *channel = pattern->GetChannel();
    SynthIndex index = channel->GetSynthIndex();
    _patch_selection_view_controller->SetSelectedAtomPatchIndex(true, index);
    _patch_selection_view_controller->Update();
  }

  for (const NoteEvent &e : _impromptu_notes) {
    Synth *synth = e.synth;
    if (synth) {
      Gate gate = synth->GetGate();
      gate += 1;
      if (gate >= kStepCapacity * kMicrosteps) {
        gate = kStepCapacity * kMicrosteps;
      }
      synth->SetGate(gate);
    }
  }
  for (Knob *knob : GetView()->GetKnobs()) {
    KnobIndex index = knob->GetKnobIndex();
    knob->SetCC(GetCurrentSynthChannel(_channel_index)->GetDefaultCC(index));
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
  SignalNoteOn(note, kDefaultVelocity);
}

void SynthViewController::ReleaseNote(const Note &note) {
  SignalNoteOff(note);
}

void SynthViewController::NoteOn(const MIDINote &note, const Velocity &velocity) {
  Note n = MIDIToNote(note, ScaleChromatic, 0);
  SignalNoteOn(n, velocity);
}

void SynthViewController::NoteOff(const MIDINote &note) {
  Note n = MIDIToNote(note, ScaleChromatic, 0);
  SignalNoteOff(n);
}

void SynthViewController::SendCC(const Control &control, const CC &cc) {
  SignalCC(control, cc);
}

void SynthViewController::SignalNoteOn(const Note &note, const Velocity &velocity) {
  if (_editing_step) {
    if (!_editing_step->RemoveNote(note)) {
      _editing_step->AddNote(note);
    }
  } else {
    Session *session = GetCurrentSession();
    Note converted_note = ConvertNoteScale(note, session->GetBaseNote().scale);
    MIDINote midi_note = NoteToMIDI(converted_note, session->GetTonicDegree());
    struct NoteEvent event = {
      .synth = nullptr,
      .midi_note = midi_note,
      .velocity = velocity,
      .note = note,
    };
    if (IsRecording()) {
      PatternChainRunner<Synth> * pattern_chain_runner = GetSynthPatternChainRunner(_channel_index);
      Step<Synth> *step = pattern_chain_runner->GetStep();
      // If exceed capacity, synth will be returned as a nullptr.
      Synth *synth = step->AddNote(note);
      if (synth) {
        synth->SetGate(0);
        synth->SetVelocity(velocity);
      }
      event.synth = synth;
    }
    _impromptu_notes.push_back(event);
    _output->NoteOn(event.midi_note, event.velocity);
  }
}

void SynthViewController::SignalNoteOff(const Note &note) {
  if (_editing_step) {
    // no-op.
  } else {
    for (std::vector<NoteEvent>::iterator it = _impromptu_notes.begin();
         it != _impromptu_notes.end();
         /* empty */) {
      if (NoteIdentical(it->note, note)) {
        _output->NoteOff(it->midi_note);
        it = _impromptu_notes.erase(it);
      } else {
        ++it;
      }
    }
  }
}

void SynthViewController::SignalCC(const Control &control, const CC &cc) {
  if (IsRecording()) {
    PatternChainRunner<Synth> * runner = GetSynthPatternChainRunner(_channel_index);
    Step<Synth> *current_step = runner->GetStep();
    current_step->RecordAutomation(control, cc);
  }
  Channel<Synth> *channel = GetCurrentSynthChannel(_channel_index);
  channel->SetDefaultCC(control, cc);
  _output->ControlChange(control, cc);
}

void SynthViewController::HandleOctUp() {
  if (_patch_selection_view_controller) {
    _patch_selection_view_controller->HandleOctUp();
  } else if (IsHoldingShift()) {
    Pattern<Synth> *pattern = GetCurrentSynthPattern(_channel_index);
    pattern->OctUp();
  } else {
    Session *session = GetCurrentSession();
    Note base_note = session->GetBaseNote();
    ++base_note.octave;
    if (base_note.octave > kMaxOctave) {
      base_note.octave = kMaxOctave;
    }
    session->SetBaseNote(base_note);
    UpdateEditingMode();
  }
}

void SynthViewController::HandleOctDown() {
  if (_patch_selection_view_controller) {
    _patch_selection_view_controller->HandleOctDown();
  } else if (IsHoldingShift()) {
    Pattern<Synth> *pattern = GetCurrentSynthPattern(_channel_index);
    pattern->OctDown();
  } else {
    Session *session = GetCurrentSession();
    Note base_note = session->GetBaseNote();
    --base_note.octave;
    if (base_note.octave < kMinOctave) {
      base_note.octave = kMinOctave;
    }
    session->SetBaseNote(base_note);
    UpdateEditingMode();
  }
}

void SynthViewController::TapPatch(const SynthIndex &index) {
  Pattern<Synth> *pattern = GetCurrentSynthPattern(_channel_index);
  Channel<Synth> *channel = pattern->GetChannel();
  SynthIndex old_patch = channel->GetSynthIndex();
  if (old_patch != index) {
    channel->SetSynthIndex(index);
    _output->ProgramChange(index);
  }
  if (IsHoldingShift() || IsRecording()) {
    return;
  }
  Note base_note = GetCurrentSession()->GetBaseNote();
  SignalNoteOn(base_note, kDefaultVelocity);
}

void SynthViewController::ReleasePatch(const SynthIndex &index) {
   if (IsHoldingShift() || IsRecording()) {
     return;
   }
   Note base_note = GetCurrentSession()->GetBaseNote();
   SignalNoteOff(base_note);
}


void SynthViewController::Change(Knob *knob, const CC &cc) {
  KnobIndex index = knob->GetKnobIndex();
  SignalCC(index, cc);
}
