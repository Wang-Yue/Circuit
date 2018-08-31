//
//  SwingViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/28/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef SwingViewController_hpp
#define SwingViewController_hpp

#include "ScreenController.hpp"
#include "TypeDefs.hpp"
#include "Knob.hpp"

class CircuitController;
class DigitView;

class SwingViewController :
public ScreenController,
public KnobDelegate {
public:
  SwingViewController(CircuitController *parent);
  virtual ~SwingViewController();
  // ScreenController.
  virtual void Update() override;
  virtual void UpdateEditingMode() override;
  virtual void UpdateRunningMode() override;
  // KnobDelegate.
  virtual void Change(Knob *, const CC &cc) override;
private:
  DigitView *_digit_view;
};

#endif /* SwingViewController_hpp */
