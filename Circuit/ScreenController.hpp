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

class CircuitView;
class CircuitController;
class SessionRunner;
class Session;
class Sample;
class Synth;
template <typename AtomClass> class PatternChainRunner;
template <typename AtomClass> class ChannelRunner;
template <typename AtomClass> class Pattern;

class ScreenController {
public:
  ScreenController(CircuitController *parent);
  virtual ~ScreenController();
  virtual void TickStep() = 0;
  CircuitView *GetView() const;
protected:
  Session *GetCurrentSession();
  SessionRunner *GetSessionRunner();
  Pattern<Sample> * GetCurrentSamplePattern(const ChannelIndex &index);
  Pattern<Synth> * GetCurrentSynthPattern(const ChannelIndex &index);
  PatternChainRunner<Sample> *GetSamplePatternChainRunner(const ChannelIndex &index);
  PatternChainRunner<Synth> *GetSynthPatternChainRunner(const ChannelIndex &index);
  ChannelRunner<Sample> *GetSampleChannelRunner(const ChannelIndex &index);
  ChannelRunner<Synth> *GetSynthChannelRunner(const ChannelIndex &index);
//  virtual void TickMicroStep() = 0;
protected:
  CircuitView *_view;
  CircuitController *_parent;
};
#endif /* ScreenController_hpp */
