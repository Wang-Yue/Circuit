//
//  KeyboardView.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef KeyboardView_hpp
#define KeyboardView_hpp

#include "Color.hpp"

#include <vector>
#include "TypeDefs.hpp"
#include "Color.hpp"

class Pad;

class KeyboardViewDelegate {
public:
  virtual void Tap(const Note &note) = 0;
  virtual void Release(const Note &note) = 0;
};

class KeyboardView : public KeyboardViewDelegate {
public:
  KeyboardView(const Note &base_note,
               const Degree &tonic_degree,
               const std::vector<Pad *> &pads,
               KeyboardViewDelegate *delegate);
  ~KeyboardView();
  std::vector<Pad *> GetPads(const Note &note);
  void AllNotesOff();
  void Highlight(const Note &note);
  virtual void Tap(const Note &note) override;
  virtual void Release(const Note &note) override;
private:
  class OneOctaveKeyboardView;
  std::vector<OneOctaveKeyboardView *> _views;
  KeyboardViewDelegate *_delegate;
};

#endif /* KeyboardView_hpp */
