//
//  LengthView.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/24/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef LengthView_hpp
#define LengthView_hpp


#include <vector>
#include <map>
#include "TypeDefs.hpp"
#include "Pad.hpp"
#include "Color.hpp"

class LengthViewDelegate {
public:
  virtual void Tap(const StepIndex &length) = 0;
};

class LengthView : public PadDelegate {
public:
  LengthView(const std::vector<Pad *> &pads, LengthViewDelegate *delegate);
  void SetLength(const StepIndex &length);
  virtual void Tap(Pad *pad) override;
  virtual void Release(Pad *pad) override;
private:
  std::vector<Pad *> _pads;
  // pad's own PadIndex -> index in _pads vector.
  std::map<PadIndex, PadIndex> _pad_index_mapping;
  LengthViewDelegate *_delegate;
};

#endif /* LengthView_hpp */
