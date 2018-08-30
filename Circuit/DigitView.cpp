//
//  DigitView.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/28/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "DigitView.hpp"
#include "Pad.hpp"

#include <array>

static const Color kDisabledColor(0x00000000);
static const Color kEnabledColor(0x007f7f7f);

static const std::array<bool, 12> off = {
  false, false, false,
  false, false, false,
  false, false, false,
  false, false, false,
};


static const std::array<bool, 12> zero = {
  true, true, true,
  true, false, true,
  true, false, true,
  true, true, true,
};

static const std::array<bool, 12> one = {
  false, true, false,
  false, true, false,
  false, true, false,
  false, true, false,
};

static const std::array<bool, 12> two = {
  false, true, true,
  false, false, true,
  false, true, false,
  false, true, true,
};

static const std::array<bool, 12> three = {
  true, true, true,
  false, true, true,
  false, false, true,
  true, true, true,
};

static const std::array<bool, 12> four = {
  true, false, true,
  true, false, true,
  true, true, true,
  false, false, true,
};

static const std::array<bool, 12> five = {
  false, true, true,
  false, true, false,
  false, false, true,
  false, true, true,
};

static const std::array<bool, 12> six = {
  false, true, false,
  false, true, false,
  false, true, true,
  false, true, true,
};

static const std::array<bool, 12> seven = {
  false, true, true,
  false, false, true,
  false, false, true,
  false, false, true,
};

static const std::array<bool, 12> eight = {
  true, true, true,
  true, true, true,
  true, false, true,
  true, true, true,
};

static const std::array<bool, 12> nine = {
  false, true, true,
  false, true, true,
  false, false, true,
  false, false, true,
};

static const std::array<std::array<bool, 12>, 10> digit_bitmaps = {
  zero, one, two, three, four, five, six, seven, eight, nine,
};


DigitView::DigitView(const std::vector<Pad *> &pads) : _pads(pads) {
  for (Pad *pad : pads) {
    pad->SetDelegate(nullptr);
  }
}


DigitView::~DigitView() {
}

void DigitView::SetNumber(const uint8_t &number) {
  uint8_t d[3];
  d[0] = number / 100;
  d[1] = number % 100 / 10;
  d[2] = number % 10;
  std::array<std::array<bool, 12>, 3> display;
  bool leading_zero = true;
  for (uint8_t i = 0; i < 3; ++i) {
    if (d[i] == 0 && leading_zero) {
      display[i] = off;
    } else {
      leading_zero = false;
      display[i] = digit_bitmaps[d[i]];
    }
  }
  for (PadIndex pad_index = 0; pad_index < _pads.size(); ++pad_index) {
    uint8_t row = pad_index / 8;
    uint8_t digit = (pad_index % 8 + 1) / 3;
    uint8_t column = (pad_index % 8 + 1) % 3;
    bool d = display[digit][row * 3 + column];
    _pads[pad_index]->SetColor(d ? kEnabledColor : kDisabledColor);
  }
}

