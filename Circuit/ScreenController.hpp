//
//  ScreenController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef ScreenController_hpp
#define ScreenController_hpp

#include "TypeDefs.hpp"
#include "UIDefs.hpp"

class CircuitView;
class CircuitController;
class SessionRunner;
class Session;
class Sample;
class Synth;
class MIDIDelegate;
template <typename AtomClass> class PatternChainRunner;
template <typename AtomClass> class ChannelRunner;
template <typename AtomClass> class Pattern;

class ScreenController {
public:
  ScreenController(CircuitController *parent);
  virtual ~ScreenController();
  virtual void Update() = 0;
  CircuitView *GetView();
  virtual void UpdateEditingMode() = 0;
  virtual void UpdateRunningMode() = 0;
  virtual void HandleOctUp();
  virtual void HandleOctDown();

protected:
  Session *GetCurrentSession();
  SessionRunner *GetSessionRunner();
  Pattern<Sample> * GetCurrentSamplePattern(const ChannelIndex &index);
  Pattern<Synth> * GetCurrentSynthPattern(const ChannelIndex &index);
  PatternChainRunner<Sample> *GetSamplePatternChainRunner(const ChannelIndex &index);
  PatternChainRunner<Synth> *GetSynthPatternChainRunner(const ChannelIndex &index);
  ChannelRunner<Sample> *GetSampleChannelRunner(const ChannelIndex &index);
  ChannelRunner<Synth> *GetSynthChannelRunner(const ChannelIndex &index);
  void SetMidiDelegate(MIDIDelegate *midi_delegate);
  bool IsHoldingShift() const;
  
  bool IsStopped() const;
  bool IsPlaying() const;
  bool IsRecording() const;
  
  enum CircuitEditingMode GetEditingMode() const;
  
  
protected:
  CircuitView *_view;
  CircuitController *_parent;
};
#endif /* ScreenController_hpp */
