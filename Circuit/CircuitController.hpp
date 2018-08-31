//
//  CircuitController.hpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef CircuitController_hpp
#define CircuitController_hpp


#include "UIDefs.hpp"
#include "Pad.hpp"
#include "MIDIDelegate.hpp"

class Workspace;
class Sample;
class Synth;
class CircuitView;
class Session;
class ScreenController;
template <typename AtomClass> class PatternChainRunner;
class SessionRunner;

class CircuitController : public PadDelegate, public MIDIDelegate {
public:
  CircuitController();
  
  ~CircuitController();
  
  CircuitView *GetView() const;

  void Restart();

  bool IsHoldingShift() const;
  bool IsFixedVelocityMode() const;

  CircuitEditingMode GetEditingMode() const;
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
  Swing GetSwing() const;
  void SetBPM(const BPM &bpm);
  void SetSwing(const Swing &swing);

  void SetMidiDelegate(MIDIDelegate *midi_delegate);
  // MIDIDelegate.
  virtual void NoteOn(const MIDINote &note, const Velocity &velocity) override;
  virtual void NoteOff(const MIDINote &note) override;
  virtual void SendCC(const Control &control, const CC &cc) override;
  // PadDelegate.
  virtual void Tap(Pad *pad) override;
  virtual void Release(Pad *) override;
  
private:
  void SetEditingMode(const CircuitEditingMode &mode);
  void SetSettingMode(const CircuitSettingMode &mode);

  void SwitchToSynth(const ChannelIndex &index);
  void SwitchToSample(const ChannelIndex &index);
  void SwitchToScaleMode();
  void SwitchToPatternChainMode();
  void SwitchToTempoMode();
  void SwitchToSwingMode();
  
  void UpdateControlPadColor();

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
  Swing _swing;
};
#endif /* CircuitController_hpp */
