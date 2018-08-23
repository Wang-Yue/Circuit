//
//  SynthViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef SynthViewController_hpp
#define SynthViewController_hpp

#include "ScreenController.hpp"

#include "SynthPatternController.hpp"
#include "KeyboardController.hpp"

#include "TypeDefs.hpp"
#include <vector>
#include <set>


class CircuitController;

class SynthPatternController;
class KeyboardController;
class Synth;

class SynthViewController : public ScreenController, public SynthPatternControllerDelegate, public KeyboardControllerDelegate {
public:
  SynthViewController(CircuitController *parent,
                      const ChannelIndex &channel,
                      const bool &expand_keyboard);
  virtual ~SynthViewController();
  virtual void TickStep() override;
  virtual void SelectStep(Step<Synth> *step) override;
  virtual void ReleaseStep() override;
  virtual void TapNote(const Note &note) override;
  virtual void ReleaseNote(const Note &note) override;
private:
  const ChannelIndex _channel_index;
  SynthPatternController *_synth_controller;
  KeyboardController *_keyboard_controller;
  std::map<Synth *, Note> _active_atoms;
};

#endif /* SynthViewController_hpp */
