//
//  TempoViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/28/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef TempoViewController_hpp
#define TempoViewController_hpp

#include "ScreenController.hpp"
#include "TypeDefs.hpp"
#include "Knob.hpp"

class CircuitController;
class DigitView;

class TempoViewController :
public ScreenController,
public KnobDelegate {
public:
  TempoViewController(CircuitController *parent);
  virtual ~TempoViewController();
  // ScreenController.
  virtual void Update() override;
  virtual void UpdateEditingMode() override;
  virtual void UpdateRunningMode() override;
  // KnobDelegate.
  virtual void Change(Knob *, const CC &cc) override;
private:
  DigitView *_digit_view;
};

#endif /* TempoViewController_hpp */
