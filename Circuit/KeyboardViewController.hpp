//
//  KeyboardViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef KeyboardViewController_hpp
#define KeyboardViewController_hpp

#include "KeyboardView.hpp"
#include <map>
#include <list>
#include "Pattern.hpp"
#include "Sample.hpp"
#include "Step.hpp"

#include "MidiController.hpp"

class KeyboardViewControllerDelegate {
public:
  virtual void TapNote(const Note &note) = 0;
  virtual void ReleaseNote(const Note &note) = 0;
};

class KeyboardViewController : public KeyboardViewDelegate, public MidiControllerDelegate {
public:
  KeyboardViewController(const Note &base_note,
                         const Degree &tonic_degree,
                         const std::vector<Pad *> &pads,
                         KeyboardViewControllerDelegate *delegate);
  virtual ~KeyboardViewController();
  
  void SetEditingStep(Step<Synth> *editing_step);
  void TickStep();
  void ShowPlayingNotes(const std::list<Note> &playing_notes);
  
  // KeyboardViewDelegate
  virtual void Tap(const Note &note) override;
  virtual void Release(const Note &note) override;

  // MidiControllerDelegate
  virtual void NoteOn(const Note &note) override;
  virtual void NoteOff(const Note &note) override;
private:
  void ShowStepNotes();

  KeyboardView *_view;
  Step<Synth> *_editing_step;
  KeyboardViewControllerDelegate *_delegate;
};

#endif /* KeyboardViewController_hpp */
