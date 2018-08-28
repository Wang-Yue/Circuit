//
//  PatternChainViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/28/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef PatternChainViewController_hpp
#define PatternChainViewController_hpp

#include "ScreenController.hpp"
#include "TypeDefs.hpp"
#include "Pad.hpp"

#include <vector>

class CircuitController;

class PatternChainViewController :
public ScreenController,
public PadDelegate {
public:
  PatternChainViewController(CircuitController *parent);
  virtual ~PatternChainViewController();
  // ScreenController.
  virtual void Update() override;
  virtual void UpdateRunningMode() override;
  virtual void UpdateEditingMode() override;
  // PadDelegate
  virtual void Tap(Pad *) override;
  virtual void Release(Pad *) override;
private:
  std::vector<Pad *> _pads;
};

#endif /* PatternChainViewController_hpp */
