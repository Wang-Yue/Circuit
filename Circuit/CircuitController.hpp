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

#include "MIDIDelegate.hpp"

class CircuitController : public PadDelegate, public MIDIDelegate {
public:
  CircuitController();
  
  ~CircuitController();
  
  CircuitView *GetView() const;

  void RestartRunning();

  bool IsHoldingShift() const;
  bool IsFixedVelocityMode() const;

  enum CircuitEditingMode GetEditingMode() const;
  void Stop();
  
  bool IsStopped() const;
  
  void Play();
  
  bool IsPlaying() const;
  void Record();
  bool IsRecording() const;
  void TickMicrostep();
  Pad *GetPad(const PadIndex &index) const;
  
  Session *GetCurrentSession();
  SessionRunner *GetSessionRunner();
  
  void SetSessionRunner(SessionRunner *runner);

  BPM GetBPM() const;

  void SetMidiDelegate(MIDIDelegate *midi_delegate);
  // MIDIDelegate.
  virtual void NoteOn(const Note &note, const Velocity &velocity) override;
  virtual void NoteOff(const Note &note) override;
  // PadDelegate.
  virtual void Tap(Pad *pad) override;
  virtual void Release(Pad *) override;
  
private:
  void SetEditingMode(const enum CircuitEditingMode &mode);
  void SetSettingMode(const enum CircuitSettingMode &mode);

  void SwitchToSynth(const ChannelIndex &index);
  void SwitchToSample(const ChannelIndex &index);
  void SwitchToScaleMode();
  
  CircuitView *_view;
  Workspace *_workspace;
  SessionRunner *_session_runner;
  ScreenController *_screen_controller;
  CircuitRunningMode _circuit_mode;
  CircuitEditingMode _editing_mode;
  CircuitSettingMode _setting_mode;
  CircuitViewMode _atom_mode;
  ChannelIndex _channel_index;
  bool _is_holding_shift;
  bool _is_fixed_velocity_mode;
  bool _recording_button_pressed;
  MIDIDelegate *_midi_delegate;
  BPM _bpm;
};
#endif /* CircuitController_hpp */
