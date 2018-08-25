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
#include "ScreenController.hpp"

#include "UIDefs.hpp"


class CircuitController {
public:
  CircuitController() {
    _circuit_mode = CircuitStopMode;
    _workspace = new Workspace();
    _editing_mode = CircuitEditNoteMode;
    _atom_mode = CircuitAtomSynth;
    _session_runner = new SessionRunner(GetCurrentSession());
    _screen_controller = new SynthViewController(this, 0);
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
    const std::vector<ChannelIndex> channels({0, 1});
    _screen_controller = new SampleViewController(this, channels);
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
  }
  
  bool IsStopped() const {
    return _circuit_mode == CircuitStopMode;
  }
  
  void Play() {
    _circuit_mode = CircuitPlayingMode;
    RestartRunning();
  }
  
  bool IsPlaying() const {
    return _circuit_mode == CircuitPlayingMode;
  }
  
  void Record() {
    _circuit_mode = CircuitRecordMode;
  }
  
  bool IsRecording() const {
    return _circuit_mode == CircuitRecordMode;
  }

  
  ~CircuitController() {
    delete _screen_controller;
    delete _session_runner;
    delete _workspace;
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
  
private:
  Workspace *_workspace;
  SessionRunner *_session_runner;
  ScreenController *_screen_controller;
  CircuitRunningMode _circuit_mode;
  CircuitEditingMode _editing_mode;
  CircuitViewMode _atom_mode;
};
#endif /* CircuitController_hpp */
