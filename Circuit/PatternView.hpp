//
//  PatternView.hpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef PatternView_hpp
#define PatternView_hpp

#include "Color.hpp"

#include <vector>
#include <map>
#include "TypeDefs.hpp"
#include "Pad.hpp"
#include "Color.hpp"

class PatternViewDelegate {
public:
  virtual void Tap(const StepIndex &step_index) = 0;
  virtual void Release(const StepIndex &step_index) = 0;
};


static const Color kCurserColor(0x000000ff);
static const Color kAvailableKeyColor(0x0000ffff);
static const Color kDisabledKeyColor(0x00000000);
static const Color kGhostKeyColor(0x000000ff); // has note but disabled.
static const Color kEnabledKeyColor(0x00ffffff);
static const Color kSelectedKeyColor(0x00ffff00);

class PatternView : public PadDelegate {
public:
  PatternView(const std::map<StepIndex, Pad *> &step_pad_mapping, PatternViewDelegate *delegate):
  _step_pad_mapping(step_pad_mapping), _delegate(delegate) {
    for (auto const& keyval : step_pad_mapping) {
      StepIndex step = keyval.first;
      Pad *pad = keyval.second;
      PadIndex pad_index = pad->GetPadIndex();
      _pad_step_mapping[pad_index] = step;
      pad->SetDelegate(this);
    }
  }
  PadIndex PadCount() const {
    return _step_pad_mapping.size();
  }
  Pad *GetPad(const StepIndex &step_index) {
    return _step_pad_mapping[step_index];
  }

  void SetDisabled(const StepIndex &step_index) {
    Pad *pad = GetPad(step_index);
    pad->SetColor(kDisabledKeyColor);
  }
  
  void SetEnabled(const StepIndex &step_index) {
    Pad *pad = GetPad(step_index);
    pad->SetColor(kEnabledKeyColor);
  }
  
  void SetAvailable(const StepIndex &step_index) {
    Pad *pad = GetPad(step_index);
    pad->SetColor(kAvailableKeyColor);
  }
  
  void SetGhost(const StepIndex &step_index) {
    Pad *pad = GetPad(step_index);
    pad->SetColor(kGhostKeyColor);
  }
  
  void SetCurser(const StepIndex &step_index) {
    Pad *pad = GetPad(step_index);
    pad->SetColor(kCurserColor);
  }
  
  void SetSelected(const StepIndex &step_index) {
    Pad *pad = GetPad(step_index);
    pad->SetColor(kSelectedKeyColor);
  }

  virtual void Tap(Pad *pad) {
    PadIndex pad_index = pad->GetPadIndex();
    StepIndex step_index = _pad_step_mapping[pad_index];
    _delegate->Tap(step_index);
  }
  virtual void Release(Pad *pad) {
    PadIndex pad_index = pad->GetPadIndex();
    StepIndex step_index = _pad_step_mapping[pad_index];
    _delegate->Release(step_index);
  }
private:
  std::map<StepIndex, Pad *> _step_pad_mapping;
  std::map<PadIndex, StepIndex> _pad_step_mapping;
  PatternViewDelegate *_delegate;
};


#endif /* PatternView_hpp */
