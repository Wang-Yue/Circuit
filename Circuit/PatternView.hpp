//
//  PatternView.hpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef PatternView_hpp
#define PatternView_hpp

#include <map>

#include "TypeDefs.hpp"
#include "Pad.hpp"

class PatternViewDelegate {
public:
  virtual void Tap(const StepIndex &step_index) = 0;
  virtual void Release(const StepIndex &step_index) = 0;
};

class PatternView : public PadDelegate {
public:
  PatternView(const std::map<StepIndex, Pad *> &step_pad_mapping, PatternViewDelegate *delegate);
  ~PatternView();
  PadIndex PadCount() const;
  Pad *GetPad(const StepIndex &step_index);
  void SetDisabled(const StepIndex &step_index);
  void SetEnabled(const StepIndex &step_index);
  void SetAvailable(const StepIndex &step_index);
  void SetGhost(const StepIndex &step_index);
  void SetCurser(const StepIndex &step_index);
  void SetSelected(const StepIndex &step_index);

  // PadDelegate
  virtual void Tap(Pad *pad) override;
  virtual void Release(Pad *pad) override;
private:
  std::map<StepIndex, Pad *> _step_pad_mapping;
  std::map<PadIndex, StepIndex> _pad_step_mapping;
  PatternViewDelegate *_delegate;
};


#endif /* PatternView_hpp */
