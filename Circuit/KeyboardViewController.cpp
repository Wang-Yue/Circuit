//
//  KeyboardViewController.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "KeyboardViewController.hpp"

KeyboardViewController::KeyboardViewController(const Note &base_note,
                                               const Degree &tonic_degree,
                                               const std::vector<Pad *> &pads,
                                               KeyboardViewControllerDelegate *delegate) : _editing_step(nullptr), _delegate(delegate) {
  _view = new KeyboardView(base_note, tonic_degree, pads, this);
  MidiController::getInstance().SetDelegate(this);
}

KeyboardViewController::~KeyboardViewController() {
  delete _view;
}

void KeyboardViewController::SetEditingStep(Step<Synth> *editing_step) {
  if (editing_step == _editing_step) {
    return;
  }
  _editing_step = editing_step;
}

void KeyboardViewController::Tap(const Note &note) {
  if (_editing_step) {
    if (!_editing_step->RemoveNote(note)) {
      _editing_step->AddNote(note);
    }
  } else {
    if (_delegate) {
      _delegate->TapNote(note);
    }
  }
}
void KeyboardViewController::Release(const Note &note) {
  if (!_editing_step) {
    if (_delegate) {
      _delegate->ReleaseNote(note);
    }
  }
}

void KeyboardViewController::TickStep() {
  _view->AllNotesOff();
  ShowStepNotes();
}

void KeyboardViewController::ShowPlayingNotes(const std::list<Note> &playing_notes) {
  for (const Note &note : playing_notes) {
    _view->Highlight(note);
  }
}

void KeyboardViewController::NoteOn(const Note &note) {
  Tap(note);
}
void KeyboardViewController::NoteOff(const Note &note) {
  Release(note);
}

void KeyboardViewController::ShowStepNotes() {
  if (_editing_step) {
    std::vector<Note> notes = _editing_step->GetNotes();
    for (Note &note : notes) {
      _view->Highlight(note);
    }
  }
}

