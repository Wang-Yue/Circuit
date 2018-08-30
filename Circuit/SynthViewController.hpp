//
//  SynthViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef SynthViewController_hpp
#define SynthViewController_hpp

#include "ScreenController.hpp"

#include "PatternViewController.hpp"
#include "KeyboardViewController.hpp"
#include "PatchSelectionViewController.hpp"

#include "TypeDefs.hpp"
#include <vector>

#include "MIDIDelegate.hpp"
#include "Knob.hpp"

class CircuitController;
class SynthChannelOutputInterface;
class SynthGateViewController;
class KeyboardViewController;
class SynthMicrostepDelayViewController;
class Synth;

template <typename AtomClass> class VelocityViewController;
template <typename AtomClass> class LengthViewController;
template <typename AtomClass> class NudgeViewController;

class SynthViewController :
public ScreenController,
public PatternViewControllerDelegate<Synth>,
public KeyboardViewControllerDelegate,
public MIDIDelegate,
public SynthPatchSelectionViewControllerDelegate,
public KnobDelegate {
  
public:
  SynthViewController(CircuitController *parent,
                      const ChannelIndex &channel);
  virtual ~SynthViewController();
  // PatternViewControllerDelegate.
  virtual void SelectStep(Step<Synth> *step, const StepIndex &selected_index) override;
  virtual void ReleaseStep(const StepIndex &selected_index) override;
  // KeyboardViewControllerDelegate.
  virtual void TapNote(const Note &note) override;
  virtual void ReleaseNote(const Note &note) override;
  // ScreenController.
  virtual void UpdateRunningMode() override;
  virtual void UpdateEditingMode() override;
  virtual void Update() override;
  void HandleOctUp() override;
  void HandleOctDown() override;
  // MidiDelegate.
  virtual void NoteOn(const MIDINote &note, const Velocity &velocity) override;
  virtual void NoteOff(const MIDINote &note) override;
  virtual void SendCC(const Control &control, const CC &cc) override;
  // SynthPatchSelectionViewControllerDelegate.
  virtual void TapPatch(const SynthIndex &index) override;
  virtual void ReleasePatch(const SynthIndex &index) override;
  // KnobDelegate.
  virtual void Change(Knob *, const CC &cc) override;
private:
  void KillAllControllers();
  void ReleaseImpromptuNotes();
  void SignalNoteOn(const Note &note, const Velocity &velocity);
  void SignalNoteOff(const Note &note);
  void SignalCC(const Control &control, const CC &cc);

  const ChannelIndex _channel_index;
  PatternViewController<Synth> *_pattern_controller;
  KeyboardViewController *_keyboard_controller;
  SynthGateViewController *_gate_view_controller;
  VelocityViewController<Synth> *_velocity_view_controller;
  LengthViewController<Synth> *_length_view_controller;
  NudgeViewController<Synth> *_nudge_view_controller;
  SynthPatchSelectionViewController *_patch_selection_view_controller;
  SynthMicrostepDelayViewController *_microstep_delay_view_controller;
  struct NoteEvent {
    Synth *synth;
    MIDINote midi_note;
    Velocity velocity;
    Note note;
  };
  // note that will not be recorded will have an empty synth pointer.
  std::vector<NoteEvent> _impromptu_notes;
  StepIndex _editing_step_index;
  Step<Synth> *_editing_step;
  SynthChannelOutputInterface *_output;
};

#endif /* SynthViewController_hpp */
