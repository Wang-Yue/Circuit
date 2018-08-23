//
//  SynthPatternController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef SynthPatternController_hpp
#define SynthPatternController_hpp

#include "PatternView.hpp"
#include <map>
#include <set>

#include "Pattern.hpp"
#include "Synth.hpp"
#include "Step.hpp"


class SynthPatternControllerDelegate {
public:
  virtual void SelectStep(Step<Synth> *step) = 0;
  virtual void ReleaseStep() = 0;
};


class SynthPatternController : PatternViewDelegate {
public:
  SynthPatternController(const std::vector<Pad *> &pads,
                         Pattern<Synth> *pattern,
                         SynthPatternControllerDelegate *delegate) : _delegate(delegate) {
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
  
  virtual ~SynthPatternController(){
    delete _view;
  }
  virtual void Tap(const StepIndex &step_index) override {
    _selected_index.insert(step_index);
    
    if (!_delegate) {
      return;
    }
    if (_selected_step) {
      _delegate->ReleaseStep();
    }
    _selected_step = _pattern->GetStep(step_index);
    _delegate->SelectStep(_selected_step);
  }
  
  virtual void Release(const StepIndex &step_index) override {
    _selected_index.erase(step_index);

    if (!_delegate) {
      return;
    }
    assert(_selected_step != nullptr);
    // One may selected a few steps. We only release the last selected one, as others are already
    // released in the Tap function.
    Step<Synth> *step = _pattern->GetStep(step_index);
    if (step == _selected_step) {
      _delegate->ReleaseStep();
    }
    _selected_step = nullptr;
  }
  
  void TickStep(const StepIndex &step_index) {
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
    _view->SetCurser(step_index);
    for (const StepIndex &index : _selected_index) {
      _view->SetSelected(index);
    }
  }
private:
  std::set<StepIndex> _selected_index;
  Step<Synth> *_selected_step;
  SynthPatternControllerDelegate *_delegate;
  PatternView *_view;
  Pattern<Synth> *_pattern;
};
#endif /* SynthPatternController_hpp */
