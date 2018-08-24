//
//  SamplePatternViewController.hpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef SamplePatternViewController_hpp
#define SamplePatternViewController_hpp

#include "PatternView.hpp"
#include <map>
#include "Pattern.hpp"
#include "Sample.hpp"
#include "Step.hpp"


class SamplePatternViewController : PatternViewDelegate {
public:
  SamplePatternViewController(const std::vector<Pad *> &pads, Pattern<Sample> *pattern) {
    std::map<StepIndex, Pad *> step_pad_mapping;
    for (StepIndex i = 0; i < pads.size(); ++i) {
      step_pad_mapping[i] = pads[i];
    }
    _view = new PatternView(step_pad_mapping, this);
    SetPattern(pattern);
  }
  
  void SetPattern(Pattern<Sample> *pattern) {
    if (pattern == _pattern) {
      return;
    }
    assert(_view->PadCount() == pattern->GetCapacity());
    _pattern = pattern;
  }
  
  virtual ~SamplePatternViewController(){
    delete _view;
  }
  virtual void Tap(const StepIndex &step_index) override {
    Step<Sample> *step = _pattern->GetStep(step_index);
    if (step->GetAtoms().size()) {
      step->RemoveAllAtoms();
    } else {
      Sample * atom = new Sample();
      step->AddAtom(atom);
    }
  }
  virtual void Release(const StepIndex &step_index) override {
    // no-op
  }
  
  void TickStep(const StepIndex &step_index) {
    StepIndex pattern_length = _pattern->GetLength();
    for (StepIndex i = 0; i < pattern_length; ++i) {
      Step<Sample> *step = _pattern->GetStep(i);
      bool note_available = step->GetAtoms().size() != 0;
      if (note_available) {
        _view->SetAvailable(i);
      } else {
        _view->SetEnabled(i);
      }
    }
    StepIndex pattern_capacity = _pattern->GetCapacity();
    for (StepIndex i = pattern_length; i < pattern_capacity; ++i) {
      Step<Sample> *step = _pattern->GetStep(i);
      bool note_available = step->GetAtoms().size() != 0;
      if (note_available) {
        _view->SetGhost(i);
      } else {
        _view->SetDisabled(i);
      }
    }
    _view->SetCurser(step_index);
  }
private:
  
  PatternView *_view;
  Pattern<Sample> *_pattern;
};

#endif /* SamplePatternViewController_hpp */
