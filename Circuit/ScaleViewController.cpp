//
//  ScaleViewController.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/26/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "ScaleViewController.hpp"
#include "Session.hpp"
#include "CircuitView.hpp"


ScaleViewController::ScaleViewController(CircuitController *parent) : ScreenController(parent) {
  Note base_note;
  base_note.degree = 0;
  base_note.octave = 0;
  base_note.scale = ScaleChromatic;
  Degree tonic_degree = GetCurrentSession()->GetTonicDegree();
  std::vector<Pad *> keyboard_pads = GetView()->GetRegularPads(0, kStepCapacity);
  _keyboard_view = new KeyboardView(base_note, tonic_degree, keyboard_pads, this);
  std::vector<Pad *> scale_pads = GetView()->GetRegularPads(kStepCapacity, kStepCapacity);
  _scale_selection_view = new ScaleSelectionView(scale_pads, this);
}

ScaleViewController:: ~ScaleViewController() {
  
}

void ScaleViewController::Update() {
  Note note;
  note.degree = GetCurrentSession()->GetTonicDegree();
  note.octave = 0;
  note.scale = ScaleChromatic;
  _keyboard_view->AllNotesOff();
  _keyboard_view->Highlight(note);
  _scale_selection_view->SetScale(GetCurrentSession()->GetBaseNote().scale);
}

void ScaleViewController::UpdateRunningMode() {
  // no-op.
}

void ScaleViewController::UpdateEditingMode() {
  // no-op.
}

void ScaleViewController::Tap(const enum Scale &scale) {
  Note note = GetCurrentSession()->GetBaseNote();
  note.scale = scale;
  GetCurrentSession()->SetBaseNote(note);
}

void ScaleViewController::Tap(const Note &note) {
  GetCurrentSession()->SetTonicDegree(note.degree);
}

void ScaleViewController::Release(const Note &note) {
  // no-op.
}
