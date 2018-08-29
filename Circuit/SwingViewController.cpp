//
//  SwingViewController.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/28/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "SwingViewController.hpp"


#include <vector>

#include "CircuitController.hpp"
#include "Pad.hpp"
#include "CircuitView.hpp"
#include "DigitView.hpp"

SwingViewController::SwingViewController(CircuitController *parent) : ScreenController(parent) {
  std::vector<Pad *> pads = GetView()->GetRegularPads(0, kRegularPadCount);
  
  _digit_view = new DigitView(pads);
  
}
SwingViewController::~SwingViewController() {
  delete _digit_view;
}

void SwingViewController::Update() {
  Swing swing = GetParent()->GetSwing();
  _digit_view->SetNumber(swing);
}

void SwingViewController::UpdateEditingMode() {
  // no-op
}

void SwingViewController::UpdateRunningMode() {
  // no-op
}
