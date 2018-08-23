//
//  KeyboardController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef KeyboardController_hpp
#define KeyboardController_hpp

#include "KeyboardView.hpp"
#include <map>
#include <list>
#include "Pattern.hpp"
#include "Sample.hpp"
#include "Step.hpp"

#include "MidiController.hpp"


class KeyboardControllerDelegate {
public:
  virtual void TapNote(const Note &note) = 0;
  virtual void ReleaseNote(const Note &note) = 0;
};

class KeyboardController : public KeyboardViewDelegate, public MidiControllerDelegate {

public:
  KeyboardController(const Note &base_note,
                     const Degree &tonic_degree,
                     const std::vector<Pad *> &pads,
                     KeyboardControllerDelegate *delegate) : _step(nullptr), _delegate(delegate) {
    _view = new KeyboardView(base_note, tonic_degree, pads, this);
    MidiController::getInstance().SetDelegate(this);
  }
  
  void SetStep(Step<Synth> *step) {
    if (step == _step) {
      return;
    }
    _step = step;
  }
  

  virtual ~KeyboardController() {
    delete _view;
  }

  virtual void Tap(const Note &note) override {
    if (_step) {
      if (!_step->RemoveNote(note)) {
        _step->AddNote(note);
      }
    } else {
      if (_delegate) {
        _delegate->TapNote(note);
      }
    }
  }
  virtual void Release(const Note &note) override {
    if (!_step) {
      if (_delegate) {
        _delegate->ReleaseNote(note);
      }
    }
  }
  
  void TickStep() {
    _view->AllNotesOff();
    ShowStepNotes();
  }

  void ShowPlayingNotes(const std::list<Note> &playing_notes) {
    for (const Note &note : playing_notes) {
      _view->Highlight(note);
    }
  }
  
  virtual void TapNote(const Note &note) override{
    Tap(note);
  }
  virtual void ReleaseNote(const Note &note) override {
    Release(note);
  }
private:
  void ShowStepNotes() {
    if (_step) {
      std::vector<Note> notes = _step->GetNotes();
      for (Note &note : notes) {
        _view->Highlight(note);
      }
    }
  }

  KeyboardView *_view;
  Step<Synth> *_step;
  KeyboardControllerDelegate *_delegate;
};
#endif /* KeyboardController_hpp */
