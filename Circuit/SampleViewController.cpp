//
//  SampleViewController.cpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "SampleViewController.hpp"
#include "CircuitView.hpp"
#include "SamplePatternViewController.hpp"
#include "SessionRunner.hpp"
#include "ChannelRunner.hpp"
#include "PatternChainRunner.hpp"

SampleViewController::SampleViewController(CircuitController *parent, const std::vector<ChannelIndex> &channels)
: ScreenController(parent) {
  for (ChannelIndex index : channels) {
    PadIndex start_index = index * kStepCapacity;
    std::vector<Pad *> pads = GetView()->GetRegularPads(start_index, kStepCapacity);
    Pattern<Sample> *pattern = GetCurrentSamplePattern(index);
    SamplePatternViewController *controller = new SamplePatternViewController(pads, pattern);
    _controllers[index] = controller;
  }
}
SampleViewController::~SampleViewController() {
  for (const std::pair<ChannelIndex, SamplePatternViewController *> pair : _controllers) {
    SamplePatternViewController *controller = pair.second;
    delete controller;
  }
}

void SampleViewController::SetupController(ChannelIndex index, SamplePatternViewController *controller) {
  controller->SetPattern(GetCurrentSamplePattern(index));
}

void SampleViewController::Update() {
  for (auto &p : _controllers) {
    ChannelIndex index = p.first;
    SamplePatternViewController *controller = p.second;
    SetupController(index, controller);
  }

  for (auto &p : _controllers) {
    ChannelIndex index = p.first;
    PatternChainRunner<Sample> * runner = GetSamplePatternChainRunner(index);
    SamplePatternViewController *controller = p.second;
    controller->TickStep(runner->GetStepCounter());
  }
}

void SampleViewController::UpdateEditingMode() {
  // TODO.
}
