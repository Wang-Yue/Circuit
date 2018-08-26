//
//  PatternViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef PatternViewController_hpp
#define PatternViewController_hpp

#include "PatternView.hpp"
#include <map>
#include <set>

#include "Pattern.hpp"
#include "Synth.hpp"
#include "Step.hpp"

template <typename AtomClass = Atom>
class PatternViewControllerDelegate {
public:
  virtual void SelectStep(Step<AtomClass> *step, const StepIndex &selected_index) = 0;
  virtual void ReleaseStep(const StepIndex &selected_index) = 0;
};

template <typename AtomClass = Atom>
class PatternViewController : PatternViewDelegate {
public:
  PatternViewController(const std::vector<Pad *> &pads,
                         Pattern<AtomClass> *pattern,
                         PatternViewControllerDelegate<AtomClass> *delegate):
  _delegate(delegate) {
    std::map<StepIndex, Pad *> step_pad_mapping;
    for (StepIndex i = 0; i < pads.size(); ++i) {
      step_pad_mapping[i] = pads[i];
    }
    _view = new PatternView(step_pad_mapping, this);
    SetPattern(pattern);
  }
  
  void SetPattern(Pattern<AtomClass> *pattern) {
    if (pattern == _pattern) {
      return;
    }
    assert(_view->PadCount() == pattern->GetCapacity());
    _pattern = pattern;
  }
  
  virtual ~PatternViewController(){
    delete _view;
  }
  virtual void Tap(const StepIndex &step_index) override {
    if (_delegate) {
      Step<AtomClass> *step = _pattern->GetStep(step_index);
      _delegate->SelectStep(step, step_index);
    }
  }
  
  virtual void Release(const StepIndex &step_index) override {
    if (_delegate) {
      _delegate->ReleaseStep(step_index);
    }
  }
  
  void SetSelectedStep(Step<AtomClass> *step, const StepIndex &index) {
    _selected_step = step;
    _selected_index = index;
  }
  
  void SetCurserStep(Step<AtomClass> *step, const StepIndex &index) {
    _curser_step = step;
    _curser_index = index;
  }
  
  void Update() {
    StepIndex pattern_length = _pattern->GetLength();
    for (StepIndex i = 0; i < pattern_length; ++i) {
      Step<AtomClass> *step = _pattern->GetStep(i);
      bool note_available = step->GetAtoms().size() != 0;
      if (note_available) {
        _view->SetAvailable(i);
      } else {
        _view->SetEnabled(i);
      }
    }
    StepIndex pattern_capacity = _pattern->GetCapacity();
    for (StepIndex i = pattern_length; i < pattern_capacity; ++i) {
      Step<AtomClass> *step = _pattern->GetStep(i);
      bool note_available = step->GetAtoms().size() != 0;
      if (note_available) {
        _view->SetGhost(i);
      } else {
        _view->SetDisabled(i);
      }
    }
    // Here we prioritize showing the selected step if exist, in case the curser index and selected
    // index are the same.
    if (_curser_step) {
      _view->SetCurser(_curser_index);
    }
    if (_selected_step) {
      _view->SetSelected(_selected_index);
    }
  }
private:
  StepIndex _selected_index;
  Step<AtomClass> *_selected_step;
  StepIndex _curser_index;
  Step<AtomClass> *_curser_step;
  PatternViewControllerDelegate<AtomClass> *_delegate;
  PatternView *_view;
  Pattern<AtomClass> *_pattern;
};
#endif /* PatternViewController_hpp */
