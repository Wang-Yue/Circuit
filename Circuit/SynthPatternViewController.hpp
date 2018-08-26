//
//  SynthPatternViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef SynthPatternViewController_hpp
#define SynthPatternViewController_hpp

#include "PatternView.hpp"
#include <map>
#include <set>

#include "Pattern.hpp"
#include "Synth.hpp"
#include "Step.hpp"


class SynthPatternViewControllerDelegate {
public:
  virtual void SelectStep(Step<Synth> *step, const StepIndex &selected_index) = 0;
  virtual void ReleaseStep(const StepIndex &selected_index) = 0;
};


class SynthPatternViewController : PatternViewDelegate {
public:
  SynthPatternViewController(const std::vector<Pad *> &pads,
                         Pattern<Synth> *pattern,
                         SynthPatternViewControllerDelegate *delegate) : _delegate(delegate) {
    std::map<StepIndex, Pad *> step_pad_mapping;
    for (StepIndex i = 0; i < pads.size(); ++i) {
      step_pad_mapping[i] = pads[i];
    }
    _view = new PatternView(step_pad_mapping, this);
    SetPattern(pattern);
  }
  
  void SetPattern(Pattern<Synth> *pattern) {
    if (pattern == _pattern) {
      return;
    }
    assert(_view->PadCount() == pattern->GetCapacity());
    _pattern = pattern;
  }
  
  virtual ~SynthPatternViewController(){
    delete _view;
  }
  virtual void Tap(const StepIndex &step_index) override {
    if (_delegate) {
      Step<Synth> *step = _pattern->GetStep(step_index);
      _delegate->SelectStep(step, step_index);
    }
  }
  
  virtual void Release(const StepIndex &step_index) override {
    if (_delegate) {
      _delegate->ReleaseStep(step_index);
    }
  }
  
  void SetSelectedStep(Step<Synth> *step, const StepIndex &index) {
    _selected_step = step;
    _selected_index = index;
  }
  
  void SetCurserStep(Step<Synth> *step, const StepIndex &index) {
    _curser_step = step;
    _curser_index = index;
  }
  
  void Update() {
    StepIndex pattern_length = _pattern->GetLength();
    for (StepIndex i = 0; i < pattern_length; ++i) {
      Step<Synth> *step = _pattern->GetStep(i);
      bool note_available = step->GetAtoms().size() != 0;
      if (note_available) {
        _view->SetAvailable(i);
      } else {
        _view->SetEnabled(i);
      }
    }
    StepIndex pattern_capacity = _pattern->GetCapacity();
    for (StepIndex i = pattern_length; i < pattern_capacity; ++i) {
      Step<Synth> *step = _pattern->GetStep(i);
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
  Step<Synth> *_selected_step;
  StepIndex _curser_index;
  Step<Synth> *_curser_step;
  SynthPatternViewControllerDelegate *_delegate;
  PatternView *_view;
  Pattern<Synth> *_pattern;
};
#endif /* SynthPatternViewController_hpp */
