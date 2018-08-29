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


class CircuitController;
class DigitView;

class TempoViewController :
public ScreenController {
public:
  TempoViewController(CircuitController *parent);
  virtual ~TempoViewController();
  // ScreenController.
  virtual void Update() override;
  virtual void UpdateEditingMode() override;
  virtual void UpdateRunningMode() override;
private:
  DigitView *_digit_view;
};

#endif /* TempoViewController_hpp */
