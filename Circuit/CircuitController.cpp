//
//  CircuitController.cpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "CircuitController.hpp"

CircuitController::CircuitController() {
  _view = new CircuitView();
  _circuit_mode = CircuitStopMode;
  _workspace = new Workspace();
  _editing_mode = CircuitEditNoteMode;
  _atom_mode = CircuitAtomSynth;
  _channel_index = 0;
  _session_runner = new SessionRunner(GetCurrentSession());
  _screen_controller = new SynthViewController(this, 0);
  _bpm = kDefaultBPM;
  _is_holding_shift = false;
  _is_fixed_velocity_mode = false;
  _recording_button_pressed = false;
  for (PadIndex index = kRegularPadCount; index < PadUnknown; ++index) {
    Pad *pad = GetView()->GetControlPad(index);
    pad->SetDelegate(this);
  }
}

CircuitController::~CircuitController() {
  delete _view;
  delete _screen_controller;
  delete _session_runner;
  delete _workspace;
}

CircuitView *CircuitController::GetView() const {
  return _view;
}


void CircuitController::RestartRunning() {
  if (_session_runner) {
    delete _session_runner;
  }
  _session_runner = new SessionRunner(GetCurrentSession());
}

bool CircuitController::IsHoldingShift() const {
  return _is_holding_shift;
}

void CircuitController::SwitchToSynth(const ChannelIndex &index){
  if (_atom_mode == CircuitAtomSynth && _channel_index == index) {
    return;
  }
  if (_screen_controller) {
    delete _screen_controller;
  }
  _atom_mode = CircuitAtomSynth;
  _channel_index = index;
  _screen_controller = new SynthViewController(this, index);
}


void CircuitController::SwitchToSample(const ChannelIndex &index) {
  if (_atom_mode == CircuitAtomSample && _channel_index == index) {
    return;
  }
  if (_screen_controller) {
    delete _screen_controller;
  }
  _atom_mode = CircuitAtomSample;
  _channel_index = index;
  _screen_controller = new SampleViewController(this, index);
}

void CircuitController::SwitchToScaleMode() {
  if (_screen_controller) {
    delete _screen_controller;
  }
  _screen_controller = new ScaleViewController(this);
}

enum CircuitEditingMode CircuitController::GetEditingMode() const {
  return _editing_mode;
}

void CircuitController::SetEditingMode(enum CircuitEditingMode &mode) {
  if (mode == _editing_mode) {
    return;
  }
  _editing_mode = mode;
  _screen_controller->UpdateEditingMode();
}

void CircuitController::Stop() {
  _circuit_mode = CircuitStopMode;
  if (_screen_controller) {
    _screen_controller->UpdateRunningMode();
  }
}

bool CircuitController::IsStopped() const {
  return _circuit_mode == CircuitStopMode;
}

void CircuitController::Play() {
  _circuit_mode = CircuitPlayingMode;
  if (_screen_controller) {
    _screen_controller->UpdateRunningMode();
  }
}

bool CircuitController::IsPlaying() const {
  return _circuit_mode == CircuitPlayingMode;
}

void CircuitController::Record() {
  _circuit_mode = CircuitRecordMode;
  if (_screen_controller) {
    _screen_controller->UpdateRunningMode();
  }
}

bool CircuitController::IsRecording() const {
  return _circuit_mode == CircuitRecordMode;
}

void CircuitController::TickMicrostep() {
  if (_screen_controller) {
    _screen_controller->Update();
  }
  if (_circuit_mode == CircuitStopMode) {
    return;
  }
  _session_runner->TickMicrostep();
}

Pad *CircuitController::GetPad(const PadIndex &index) const {
  if (!_screen_controller) {
    return nullptr;
  }
  CircuitView *view = _screen_controller->GetView();
  if (view) {
    return view->GetPad(index);
  }
  return nullptr;
}

Session *CircuitController::GetCurrentSession() {
  return _workspace->CurrentSession();
}

SessionRunner *CircuitController::GetSessionRunner() {
  return _session_runner;
}

void CircuitController::SetSessionRunner(SessionRunner *runner) {
  _session_runner = runner;
}

void CircuitController::Tap(Pad *pad){
  PadIndex index = pad->GetPadIndex();
  if (index == PadShift) {
    _is_holding_shift = true;
    return;
  }
  if (index == PadNote) {
    if (_is_holding_shift) {
      _editing_mode = CircuitEditExpandNoteMode;
    } else {
      _editing_mode = CircuitEditNoteMode;
    }
  }
  if (index == PadGate) {
      _editing_mode = CircuitEditGateMode;
  }
  if (index == PadVelocity) {
    if (_is_holding_shift) {
      _is_fixed_velocity_mode = !_is_fixed_velocity_mode;
    } else {
      _editing_mode = CircuitEditVelocityMode;
    }
  }
  if (index == PadNudge) {
    _editing_mode = CircuitEditNudgeMode;
  }
  if (index == PadLength) {
    _editing_mode = CircuitEditLengthMode;
  }
  if (index == PadScale) {
    SwitchToScaleMode();
    return;
  }
  if (index == PadRecord) {
    _recording_button_pressed = !_recording_button_pressed;
    if (_circuit_mode == CircuitPlayingMode) {
      Record();
    } else if (_circuit_mode == CircuitRecordMode) {
      Play();
    }
  }
  if (index == PadPlay) {
    if (_circuit_mode == CircuitStopMode) {
      RestartRunning();
      if (_recording_button_pressed) {
        Record();
      } else {
        Play();
      }
    } else {
      Stop();
    }
  }
  if (index == PadSynth1 || index == PadSynth2) {
    SwitchToSynth(index - PadSynth1);
    return;
  }

  if (index >= PadDrum1 && index <= PadDrum4) {
    SwitchToSample(index - PadDrum1);
    return;
  }
  if (!_screen_controller) {
    return;
  }
  if (index >= PadNote && index <= PadLength) {
    _screen_controller->UpdateEditingMode();
  }
  
}
void CircuitController::Release(Pad *pad) {
  PadIndex index = pad->GetPadIndex();
  if (index == PadShift) {
    _is_holding_shift = false;
    return;
  }
}

void CircuitController::SetMidiDelegate(MIDIDelegate *midi_delegate) {
  _midi_delegate = midi_delegate;
}

// MIDIDelegate.
void CircuitController::NoteOn(const Note &note, const Velocity &velocity) {
  if (_midi_delegate) {
    _midi_delegate->NoteOn(note, velocity);
  }
}

void CircuitController::NoteOff(const Note &note) {
  if (_midi_delegate) {
    _midi_delegate->NoteOff(note);
  }
}

BPM CircuitController::GetBPM() const {
  return _bpm;
}

