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

class CircuitController {
public:
  CircuitController() {
    _tick_step = 0;
    _workspace = new Workspace();
    _session_runner = nullptr;
    _screen_controller = nullptr;
  }
  

  void RestartRunning() {
    if (_session_runner) {
      delete _session_runner;
    }
    _session_runner = new SessionRunner(GetCurrentSession());
    _session_runner->Restart();
    // const std::vector<ChannelIndex> channels({0, 1});
    //_screen_controller = new SampleViewController(this, channels);
    _screen_controller = new SynthViewController(this, 0, false);
  }

  ~CircuitController() {
    delete _screen_controller;
    delete _session_runner;
    delete _workspace;
  }


  void TickStep() {
    // Get user input.
    
    if (_tick_step >= kStepCapacity) {
      _tick_step = 0;
    }
    _screen_controller->TickStep();
    for (int i = 0; i < kMicrosteps; ++i) {
      _session_runner->TickMicrostep();
    }
    ++_tick_step;
  }
  
  Pad *GetPad(const PadIndex &index) const {
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
  StepIndex _tick_step;

};
#endif /* CircuitController_hpp */
