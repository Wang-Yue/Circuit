//
//  NudgeView.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/25/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef NudgeView_hpp
#define NudgeView_hpp

#include <vector>
#include <map>
#include "TypeDefs.hpp"
#include "Pad.hpp"
#include "Color.hpp"

class NudgeViewDelegate {
public:
  virtual void NudgeLeft() = 0;
  virtual void NudgeRight() = 0;
};

class NudgeView : public PadDelegate {
public:
  NudgeView(const std::vector<Pad *> &pads, NudgeViewDelegate *delegate);
  virtual void Tap(Pad *pad) override;
  virtual void Release(Pad *pad) override;
private:
  Pad *_nudge_left_pad;
  Pad *_nudge_right_pad;
  NudgeViewDelegate *_delegate;
};
#endif /* NudgeView_hpp */
