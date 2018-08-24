//
//  SampleViewController.hpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef SampleViewController_hpp
#define SampleViewController_hpp

#include "ScreenController.hpp"

#include "TypeDefs.hpp"
#include <vector>
#include <map>

class CircuitController;

class SamplePatternViewController;
class Sample;

class SampleViewController : public ScreenController {
public:
  SampleViewController(CircuitController *parent, const std::vector<ChannelIndex> &channels);
  virtual ~SampleViewController();


  virtual void TickStep() override;

private:
  void SetupController(ChannelIndex index, SamplePatternViewController *controller);
  // TODO: make a base view controller.
  std::map<ChannelIndex, SamplePatternViewController *> _controllers;
};
#endif /* SampleViewController_hpp */
