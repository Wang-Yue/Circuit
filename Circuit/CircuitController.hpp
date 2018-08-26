//
//  CircuitController.hpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef CircuitController_hpp
#define CircuitController_hpp

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
#include "ScreenController.hpp"

#include "UIDefs.hpp"



class CircuitController : public PadDelegate {
public:
  CircuitController() {
    _view = new CircuitView();
    _circuit_mode = CircuitStopMode;
    _workspace = new Workspace();
    _editing_mode = CircuitEditNoteMode;
    _atom_mode = CircuitAtomSynth;
    _session_runner = new SessionRunner(GetCurrentSession());
    _screen_controller = new SynthViewController(this, 0);
    
    for (PadIndex index = kRegularPadCount; index < PadUnknown; ++index) {
      Pad *pad = GetView()->GetControlPad(index);
      pad->SetDelegate(this);
    }
  }
  
  ~CircuitController() {
    delete _view;
    delete _screen_controller;
    delete _session_runner;
    delete _workspace;
  }
  
  CircuitView *GetView() const {
    return _view;
  }

  
  void RestartRunning() {
    if (_session_runner) {
      delete _session_runner;
    }
    _session_runner = new SessionRunner(GetCurrentSession());
    _session_runner->Restart();
  }

  void SwitchToSynth() {
    if (_atom_mode == CircuitAtomSynth) {
      return;
    }
    if (_screen_controller) {
      delete _screen_controller;
    }
    _screen_controller = new SynthViewController(this, 0);
  }
  
  void SwitchToSample() {
    if (_atom_mode == CircuitAtomSample) {
      return;
    }
    if (_screen_controller) {
      delete _screen_controller;
    }
    _screen_controller = new SampleViewController(this, 0);
  }
 
  void SwitchToScaleMode() {
    if (_screen_controller) {
      delete _screen_controller;
    }
    _screen_controller = new ScaleViewController(this);
  }
  
  enum CircuitEditingMode GetEditingMode() const {
    return _editing_mode;
  }
  
  void SetEditingMode(enum CircuitEditingMode &mode) {
    if (mode == _editing_mode) {
      return;
    }
    _editing_mode = mode;
    _screen_controller->UpdateEditingMode();
  }
  
  void Stop() {
    _circuit_mode = CircuitStopMode;
    if (_screen_controller) {
      _screen_controller->UpdateRunningMode();
    }
  }
  
  bool IsStopped() const {
    return _circuit_mode == CircuitStopMode;
  }
  
  void Play() {
    _circuit_mode = CircuitPlayingMode;
    RestartRunning();
    if (_screen_controller) {
      _screen_controller->UpdateRunningMode();
    }
  }
  
  bool IsPlaying() const {
    return _circuit_mode == CircuitPlayingMode;
  }
  
  void Record() {
    _circuit_mode = CircuitRecordMode;
    if (_screen_controller) {
      _screen_controller->UpdateRunningMode();
    }
  }
  
  bool IsRecording() const {
    return _circuit_mode == CircuitRecordMode;
  }

  void TickStep() {
    if (_screen_controller) {
      _screen_controller->Update();
    }
    if (_circuit_mode == CircuitStopMode) {
      return;
    }
    for (int i = 0; i < kMicrosteps; ++i) {
      _session_runner->TickMicrostep();
    }
  }
  
  Pad *GetPad(const PadIndex &index) const {
    if (!_screen_controller) {
      return nullptr;
    }
    CircuitView *view = _screen_controller->GetView();
    if (view) {
      return view->GetPad(index);
    }
    return nullptr;
  }
  
  Session *GetCurrentSession() {
    return _workspace->CurrentSession();
  }

  SessionRunner *GetSessionRunner() {
    return _session_runner;
  }
  
  void SetSessionRunner(SessionRunner *runner) {
    _session_runner = runner;
  }
  
  virtual void Tap(Pad *pad) override {
    PadIndex index = pad->GetPadIndex();
    if (index == PadNote) {
      _editing_mode = CircuitEditNoteMode;
    }
    if (index == PadGate) {
      _editing_mode = CircuitEditGateMode;
    }
    if (index == PadVelocity) {
      _editing_mode = CircuitEditVelocityMode;
    }
    if (index == PadNudge) {
      _editing_mode = CircuitEditNudgeMode;
    }
    if (index == PadLength) {
      _editing_mode = CircuitEditLengthMode;
    }
    if (index == PadScale) {
      SwitchToScaleMode();
    }
    if (index == PadRecord) {
      if (_circuit_mode == CircuitPlayingMode) {
        Record();
      }
    }
    if (index == PadPlay) {
      if (_circuit_mode == CircuitStopMode) {
        Play();
      } else {
        Stop();
      }
    }
    if (!_screen_controller) {
      return;
    }
    if (index >= PadNote && index <= PadLength) {
      _screen_controller->UpdateEditingMode();
    }

  }
  virtual void Release(Pad *) override {
    // no-op.
  };

  
private:
  CircuitView *_view;
  Workspace *_workspace;
  SessionRunner *_session_runner;
  ScreenController *_screen_controller;
  CircuitRunningMode _circuit_mode;
  CircuitEditingMode _editing_mode;
  CircuitViewMode _atom_mode;
};
#endif /* CircuitController_hpp */
