//
//  CircuitController.cpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "CircuitController.hpp"

#include "CircuitView.hpp"
#include "Workspace.hpp"
#include "Session.hpp"
#include "Channel.hpp"
#include "Step.hpp"
#include "Sample.hpp"
#include "Synth.hpp"
#include "PatternChain.hpp"
#include "SessionRunner.hpp"
#include "ChannelRunner.hpp"
#include "SampleViewController.hpp"
#include "SynthViewController.hpp"
#include "ScaleViewController.hpp"
#include "PatternChainViewController.hpp"
#include "TempoViewController.hpp"
#include "SwingViewController.hpp"
#include "ScreenController.hpp"


CircuitController::CircuitController() {
  _view = new CircuitView();
  _circuit_mode = CircuitStopMode;
  _workspace = new Workspace();
  _editing_mode = CircuitEditNoteMode;
  _setting_mode = CircuitSetRegularMode;
  _atom_mode = CircuitAtomSynth;
  _channel_index = 0;
  _session_runner = new SessionRunner(GetCurrentSession());
  _screen_controller = new SynthViewController(this, 0);
  _bpm = kDefaultBPM;
  _swing = kDefaultSwing;
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


void CircuitController::Restart() {
  _session_runner->Restart();
}

bool CircuitController::IsHoldingShift() const {
  return _is_holding_shift;
}

bool CircuitController::IsFixedVelocityMode() const {
  return _is_fixed_velocity_mode;
}

void CircuitController::SwitchToSynth(const ChannelIndex &index){
  if (_atom_mode == CircuitAtomSynth && _channel_index == index && _setting_mode == CircuitSetRegularMode) {
    return;
  }
  if (_screen_controller) {
    delete _screen_controller;
  }
  if (_atom_mode != CircuitAtomSynth) {
    _atom_mode = CircuitAtomSynth;
    _editing_mode = CircuitEditNoteMode;
  }
  _channel_index = index;
  _setting_mode = CircuitSetRegularMode;
  _screen_controller = new SynthViewController(this, index);
}

void CircuitController::SwitchToSample(const ChannelIndex &index) {
  if (_atom_mode == CircuitAtomSample && _channel_index == index && _setting_mode == CircuitSetRegularMode) {
    return;
  }
  if (_screen_controller) {
    delete _screen_controller;
  }
  if (_atom_mode != CircuitAtomSample) {
    _atom_mode = CircuitAtomSample;
    _editing_mode = CircuitEditNoteMode;
  }
  _setting_mode = CircuitSetRegularMode;
  _channel_index = index;
  _screen_controller = new SampleViewController(this, index);
}

void CircuitController::SwitchToScaleMode() {
  if (_setting_mode == CircuitSetScaleMode) {
    return;
  }
  if (_screen_controller) {
    delete _screen_controller;
  }
  _setting_mode = CircuitSetScaleMode;
  _screen_controller = new ScaleViewController(this);
}

void CircuitController::SwitchToPatternChainMode() {
  if (_setting_mode == CircuitSetPatternChainMode) {
    return;
  }
  if (_screen_controller) {
    delete _screen_controller;
  }
  _setting_mode = CircuitSetPatternChainMode;
  _screen_controller = new PatternChainViewController(this);
}

void CircuitController::SwitchToTempoMode() {
  if (_setting_mode == CircuitSetTempoMode) {
    return;
  }
  if (_screen_controller) {
    delete _screen_controller;
  }
  _setting_mode = CircuitSetTempoMode;
  _screen_controller = new TempoViewController(this);
}

void CircuitController::SwitchToSwingMode() {
  if (_setting_mode == CircuitSetSwingMode) {
    return;
  }
  if (_screen_controller) {
    delete _screen_controller;
  }
  _setting_mode = CircuitSetSwingMode;
  _screen_controller = new SwingViewController(this);
}

CircuitEditingMode CircuitController::GetEditingMode() const {
  // Cheat the view controller.
  if (_setting_mode == CircuitSetPatchMode) {
    return CircuitEditPatchMode;
  }
  return _editing_mode;
}

void CircuitController::SetEditingMode(const CircuitEditingMode &mode) {
  if (mode == GetEditingMode()) {
    return;
  }
  // Cheat the view controller.
  if (mode == CircuitEditPatchMode) {
    _setting_mode = CircuitSetPatchMode;
  } else {
    _setting_mode = CircuitSetRegularMode;
    _editing_mode = mode;
  }
  _screen_controller->UpdateEditingMode();
}

void CircuitController::Stop() {
  _circuit_mode = CircuitStopMode;
  if (_screen_controller) {
    _screen_controller->UpdateRunningMode();
  }
  GetSessionRunner()->Stop();
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
  UpdateControlPadColor();
  if (_screen_controller) {
    _screen_controller->Update();
  }
  if (_circuit_mode == CircuitStopMode) {
    return;
  }
  _session_runner->TickMicrostep();
}

void CircuitController::UpdateControlPadColor() {
  static const Color kRecordingEnabledColor(0x000000ff);
  static const Color kRecordingAvailableColor(0x0000003f);
  GetPad(PadRecord)->SetColor(_recording_button_pressed ?
                                kRecordingEnabledColor : kRecordingAvailableColor);
 
  static const Color kPlayingEnabledColor(0x0000ff00);
  static const Color kPlayingAvailableColor(0x00003f00);
  GetPad(PadPlay)->SetColor(IsStopped() ? kPlayingAvailableColor: kPlayingEnabledColor);

  static const Color kChannelEnabledColor(0x00007f7f);
  static const Color kChannelAvailableColor(0x00003f3f);

  static const Color kEditEnabledColor(0x0000ff7f);
  static const Color kEditAvailableColor(0x00003f3f);

  if (_setting_mode == CircuitSetRegularMode || _setting_mode == CircuitSetPatchMode) {
    GetPad(PadSynth1)->SetColor(_channel_index == 0 && _atom_mode == CircuitAtomSynth ?
                                kChannelEnabledColor : kChannelAvailableColor);
    GetPad(PadSynth2)->SetColor(_channel_index == 1 && _atom_mode == CircuitAtomSynth ?
                                kChannelEnabledColor : kChannelAvailableColor);
    GetPad(PadDrum1)->SetColor(_channel_index == 0 && _atom_mode == CircuitAtomSample ?
                                kChannelEnabledColor : kChannelAvailableColor);
    GetPad(PadDrum2)->SetColor(_channel_index == 1 && _atom_mode == CircuitAtomSample ?
                                kChannelEnabledColor : kChannelAvailableColor);
    GetPad(PadDrum3)->SetColor(_channel_index == 2 && _atom_mode == CircuitAtomSample ?
                                kChannelEnabledColor : kChannelAvailableColor);
    GetPad(PadDrum4)->SetColor(_channel_index == 3 && _atom_mode == CircuitAtomSample ?
                                kChannelEnabledColor : kChannelAvailableColor);
    
    GetPad(PadNote)->SetColor(GetEditingMode() == CircuitEditNoteMode ||
                              GetEditingMode() == CircuitEditExpandNoteMode ?
                              kEditEnabledColor : kEditAvailableColor);
    GetPad(PadGate)->SetColor(GetEditingMode() == CircuitEditGateMode ||
                              GetEditingMode() == CircuitEditSynthMicrostepDelayMode ?
                              kEditEnabledColor : kEditAvailableColor);
    GetPad(PadVelocity)->SetColor(GetEditingMode() == CircuitEditVelocityMode ?
                                  kEditEnabledColor : kEditAvailableColor);
    GetPad(PadNudge)->SetColor(GetEditingMode() == CircuitEditNudgeMode ?
                               kEditEnabledColor : kEditAvailableColor);
    GetPad(PadLength)->SetColor(GetEditingMode() == CircuitEditLengthMode ?
                                kEditEnabledColor : kEditAvailableColor);
  } else {
    GetPad(PadSynth1)->SetColor(kChannelAvailableColor);
    GetPad(PadSynth2)->SetColor(kChannelAvailableColor);
    GetPad(PadDrum1)->SetColor(kChannelAvailableColor);
    GetPad(PadDrum2)->SetColor(kChannelAvailableColor);
    GetPad(PadDrum3)->SetColor(kChannelAvailableColor);
    GetPad(PadDrum4)->SetColor(kChannelAvailableColor);
    
    GetPad(PadNote)->SetColor(kEditAvailableColor);
    GetPad(PadGate)->SetColor(kEditAvailableColor);
    GetPad(PadVelocity)->SetColor(kEditAvailableColor);
    GetPad(PadNudge)->SetColor(kEditAvailableColor);
    GetPad(PadLength)->SetColor(kEditAvailableColor);
  }
  
  // TODO: Screen view responsibility
  static const Color kOctaveAvailableColor(0x00007f7f);
  static const Color kOctaveDisabledColor(0x00003f3f);

  GetPad(PadOctUp)->SetColor(kOctaveAvailableColor);
  GetPad(PadOctDown)->SetColor(kOctaveAvailableColor);
  
  static const Color kHoldingShiftColor(0x0000ff7f);
  static const Color kReleasingShiftColor(0x00007f3f);
  GetPad(PadShift)->SetColor(IsHoldingShift()? kHoldingShiftColor : kReleasingShiftColor);

  static const Color kPatternEnabledColor(0x0000ffff);
  static const Color kPatternAvailableColor(0x00007f7f);
  GetPad(PadPatterns)->SetColor(_setting_mode == CircuitSetPatternChainMode ?
                                kPatternEnabledColor : kPatternAvailableColor);

  static const Color kScaleEnabledColor(0x0000ffff);
  static const Color kScaleAvailableColor(0x00007f7f);
  GetPad(PadScale)->SetColor(_setting_mode == CircuitSetScaleMode ?
                                kScaleEnabledColor : kScaleAvailableColor);

  static const Color kTempoEnabledColor(0x0000ffff);
  static const Color kTempoAvailableColor(0x00007f7f);
  GetPad(PadTempo)->SetColor(_setting_mode == CircuitSetTempoMode ?
                             kTempoEnabledColor : kTempoAvailableColor);
  
  static const Color kSwingEnabledColor(0x0000ffff);
  static const Color kSwingAvailableColor(0x00007f7f);
  GetPad(PadSwing)->SetColor(_setting_mode == CircuitSetSwingMode ?
                             kSwingEnabledColor : kSwingAvailableColor);
  
  static const Color kMixerEnabledColor(0x0000ffff);
  static const Color kMixerAvailableColor(0x00007f7f);
  GetPad(PadSwing)->SetColor(_setting_mode == CircuitSetMixerMode ?
                             kSwingEnabledColor : kSwingAvailableColor);

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
//    _is_holding_shift = !_is_holding_shift;
    return;
  }
  if (index == PadNote) {
    if (IsHoldingShift()) {
      SetEditingMode(CircuitEditExpandNoteMode);
    } else {
      SetEditingMode(CircuitEditNoteMode);
    }
  }
  if (index == PadGate) {
    if (IsHoldingShift()){
      SetEditingMode(CircuitEditSynthMicrostepDelayMode);
    } else {
      SetEditingMode(CircuitEditGateMode);
    }
  }
  if (index == PadVelocity) {
    if (IsHoldingShift()) {
      _is_fixed_velocity_mode = !_is_fixed_velocity_mode;
    } else {
      SetEditingMode(CircuitEditVelocityMode);
    }
  }
  if (index == PadNudge) {
    SetEditingMode(CircuitEditNudgeMode);
  }
  if (index == PadLength) {
    SetEditingMode(CircuitEditLengthMode);
  }
  if (index == PadScale) {
    SwitchToScaleMode();
    return;
  }
  if (index == PadPatterns) {
    SwitchToPatternChainMode();
    return;
  }
  if (index == PadTempo) {
    SwitchToTempoMode();
    return;
  }
  if (index == PadSwing) {
    SwitchToSwingMode();
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
      Restart();
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
    if (_is_holding_shift) {
      SetEditingMode(CircuitEditPatchMode);
    }
    return;
  }

  if (index >= PadDrum1 && index <= PadDrum4) {
    SwitchToSample(index - PadDrum1);
    if (_is_holding_shift) {
      SetEditingMode(CircuitEditPatchMode);
    }
    return;
  }
  if (!_screen_controller) {
    return;
  }
  if (index == PadOctUp) {
    _screen_controller->HandleOctUp();
  }
  if (index == PadOctDown) {
    _screen_controller->HandleOctDown();
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
void CircuitController::NoteOn(const MIDINote &note, const Velocity &velocity) {
  if (_midi_delegate) {
    _midi_delegate->NoteOn(note, IsFixedVelocityMode() ? kDefaultVelocity :  velocity);
  }
}

void CircuitController::NoteOff(const MIDINote &note) {
  if (_midi_delegate) {
    _midi_delegate->NoteOff(note);
  }
}

void CircuitController::SendCC(const Control &control, const CC &cc) {
  if (_midi_delegate) {
    _midi_delegate->SendCC(control, cc);
  }
}


BPM CircuitController::GetBPM() const {
  return _bpm;
}

Swing CircuitController::GetSwing() const {
  return _swing;
}

void CircuitController::SetBPM(const BPM &bpm) {
  _bpm = bpm;
}
void CircuitController::SetSwing(const Swing &swing) {
  _swing = swing;
}

