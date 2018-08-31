//
//  DigitView.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/28/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef DigitView_hpp
#define DigitView_hpp

#include <vector>
#include <cstdint>

class Pad;

class DigitView {
public:
  DigitView(const std::vector<Pad *> &pads);
  ~DigitView();
  void SetNumber(const uint8_t &number);
private:
  std::vector<Pad *> _pads;
};

#endif /* DigitView_hpp */
