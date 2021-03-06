//
//  Channel.hpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef PatternBank_hpp
#define PatternBank_hpp

#include <vector>

#include "Pattern.hpp"
#include "PatternChain.hpp"

template <typename AtomClass> class Step;
class Atom;
template <typename AtomClass> class Channel;
class Session;

template <typename AtomClass = Atom>
class ChannelBase {
public:
  // Warning! The parent is responsible to call Initialize class afterwards.
  ChannelBase(Session *parent, const ChannelIndex &channel_index) :
  ChannelBase(kPatternCapacity, parent, channel_index) {
  }

  const ChannelIndex GetChannelIndex() const {
    return _channel_index;
  }
  
  virtual ~ChannelBase() {
    for (PatternIndex i = 0; i < _patterns.size(); ++i) {
      Pattern<AtomClass> *pattern = _patterns[i];
      delete pattern;
    }
    delete _pattern_chain;
  }

  Session *GetSession() {
    return _session;
  }
  
  // NotNull
  PatternChain<AtomClass> *GetPatternChain() const {
    return _pattern_chain;
  }

  // NotNull
  Pattern<AtomClass> *GetPattern(const PatternIndex &pattern_index) const {
    if (pattern_index >= _patterns.size()) {
      throw std::out_of_range("Exceed maximum pattern steps.");
    }
    return _patterns[pattern_index];
  }
  // NotNull
  Step<AtomClass> *GetStep(const PatternIndex &pattern_index, const StepIndex &step_index) const {
    Pattern<AtomClass> *pattern = GetPattern(pattern_index);
    return pattern->GetStep(step_index);
  }
  
  // Unfortunately we have to move the pattern generation step out of constructor, because C++
  // cannot translate this pointer to child pointer, which will be passed into the Pattern
  // constructor.
  void InitializeChannel() {
    _patterns.reserve(_pattern_capacity);
    Pattern<AtomClass> *initial_pattern = nullptr;
    for (PatternIndex i = 0; i < _pattern_capacity; ++i) {
      Pattern<AtomClass> *pattern = new Pattern<AtomClass>(GetThisPointer(), i);
      if (i == 0) {
        initial_pattern = pattern;
        _pattern_chain = new PatternChain<AtomClass>(initial_pattern);
      }
      _patterns.push_back(pattern);
    }
  }
  CC GetDefaultCC(const Control &control) const {
    assert(control < kControlCapacity);
    return _default_control_values[control];
  }
  
  void SetDefaultCC(const Control &control, const CC &cc) {
    assert(control < kControlCapacity);
    assert(cc <= kMaxCC);
    _default_control_values[control] = cc;
  }
  
protected:
  virtual Channel<AtomClass> *GetThisPointer() = 0;
private:
  ChannelBase(const PatternIndex &pattern_capacity,
              Session *session,
              const ChannelIndex &channel_index) :
  _pattern_capacity(pattern_capacity), _session(session), _channel_index(channel_index) {
    for (Control i = 0; i < kControlCapacity; ++i) {
      _default_control_values.push_back(0);
    }
  }
  PatternChain<AtomClass> *_pattern_chain;
  std::vector<Pattern<AtomClass> *> _patterns;
  const PatternIndex _pattern_capacity;
  const ChannelIndex _channel_index;
  Session *_session;
  std::vector<CC> _default_control_values;
};


template <typename AtomClass>
class Channel : public ChannelBase<AtomClass> {
};

template <>
class Channel<Sample> : public ChannelBase<Sample> {
public:
  Channel(Session *session, const ChannelIndex &channel_index) :
  ChannelBase<Sample>(session, channel_index), _sample_index(0) {
  }
  void SetSampleIndex(const SampleIndex &sample_index) {
    _sample_index = sample_index;
  }
  SampleIndex GetSampleIndex() {
    return _sample_index;
  }
private:
  virtual Channel<Sample> *GetThisPointer() override {
    return this;
  }
  SampleIndex _sample_index;
};

template <>
class Channel<Synth> : public ChannelBase<Synth> {
public:
  Channel(Session *session, const ChannelIndex &channel_index) :
  ChannelBase<Synth>(session, channel_index), _synth_index(0) {

  }
  void SetSynthIndex(const SynthIndex &synth_index) {
    _synth_index = synth_index;
  }
  SynthIndex GetSynthIndex() {
    return _synth_index;
  }
private:
  virtual Channel<Synth> *GetThisPointer() override {
    return this;
  }
  SynthIndex _synth_index;
};

#endif /* PatternBank_hpp */
