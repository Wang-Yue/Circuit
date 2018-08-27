//
//  KeyboardViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef KeyboardViewController_hpp
#define KeyboardViewController_hpp

#include <list>

#include "KeyboardView.hpp"

class Synth;
template <typename AtomClass> class Step;
template <typename AtomClass> class Pattern;

class KeyboardViewControllerDelegate {
public:
  virtual void TapNote(const Note &note) = 0;
  virtual void ReleaseNote(const Note &note) = 0;
};

class KeyboardViewController : public KeyboardViewDelegate {
public:
  KeyboardViewController(const Note &base_note,
                         const Degree &tonic_degree,
                         const std::vector<Pad *> &pads,
                         KeyboardViewControllerDelegate *delegate);
  virtual ~KeyboardViewController();
  
  void SetEditingStep(Step<Synth> *editing_step);
  void SetPlayingNotes(const std::list<Note> &playing_notes);

  void Update();
  
  // KeyboardViewDelegate
  virtual void Tap(const Note &note) override;
  virtual void Release(const Note &note) override;

private:
  KeyboardView *_view;
  Step<Synth> *_editing_step;
  std::list<Note> _playing_notes;
  KeyboardViewControllerDelegate *_delegate;
};

#endif /* KeyboardViewController_hpp */
