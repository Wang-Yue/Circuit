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


class CircuitController;
class DigitView;

class SwingViewController :
public ScreenController {
public:
  SwingViewController(CircuitController *parent);
  virtual ~SwingViewController();
  // ScreenController.
  virtual void Update() override;
  virtual void UpdateEditingMode() override;
  virtual void UpdateRunningMode() override;
private:
  DigitView *_digit_view;
};

#endif /* SwingViewController_hpp */
