//
//  Pattern.hpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef Pattern_hpp
#define Pattern_hpp

#include <vector>

#include "TypeDefs.hpp"


class Atom;
template <typename AtomClass> class Step;
template <typename AtomClass> class Channel;


template <typename AtomClass = Atom>
class Pattern {
public:

  Pattern(Channel<AtomClass> * const channel, const PatternIndex &pattern_index) :
  Pattern(kStepCapacity, channel, pattern_index) {
  }
  virtual ~Pattern()  {
    for (StepIndex i = 0; i < _steps.size(); ++i) {
      Step<AtomClass> *step = _steps[i];
      delete step;
    }
  }

  PatternIndex GetPatternIndex() const {
    return _pattern_index;
  }
  void SetLength(const StepIndex &length)  {
    if (length > _pattern_capacity) {
      throw std::out_of_range("Pattern length should be less than the capacity.");
    }
    if (length == 0) {
      throw std::out_of_range("Pattern length not be zero.");
    }
    _length = length;
  }
  
  StepIndex GetLength() const  {
    return _length;
  }

  StepIndex GetCapacity() const  {
    return _pattern_capacity;
  }

  Step<AtomClass> *GetStep(const StepIndex &index) const  {
    if (index >= _pattern_capacity) {
      throw std::out_of_range("Exceed maximum microstep.");
    }
    return _steps[index];
  }
  
  void ClearAutomation(const Control &control)  {
    for (Step<AtomClass> *step : _steps) {
      step->ClearAutomation(control);
    }
  }
  
  void NudgeLeft() {
    Step<AtomClass> *first = _steps[0];
    for (StepIndex i = 0; i < _length - 1; ++i) {
      _steps[i] = _steps[i + 1];
    }
    _steps[_length - 1] = first;
  }

  void NudgeRight() {
    Step<AtomClass> *last = _steps[_length - 1];
    for (StepIndex i = _length - 1; i > 0; --i) {
      _steps[i] = _steps[i - 1];
    }
    _steps[0] = last;
  }
  
  void OctUp() {
    for (Step<AtomClass> *step : _steps) {
      step->OctUp();
    }
  }
  
  void OctDown() {
    for (Step<AtomClass> *step : _steps) {
      step->OctDown();
    }
  }
  Channel<AtomClass> *GetChannel() {
    return _channel;
  }
private:
  Pattern(const StepIndex &pattern_capacity,
          Channel<AtomClass> * const channel,
          const PatternIndex &pattern_index) :
  _pattern_capacity(pattern_capacity), _channel(channel), _pattern_index(pattern_index) {
    _steps.reserve(pattern_capacity);
    for (StepIndex i = 0; i < pattern_capacity; ++i) {
      Step<AtomClass> *step = new Step<AtomClass>(this);
      _steps.push_back(step);
    }
    SetLength(kStepCapacity);
  }
  const PatternIndex _pattern_index;
  std::vector<Step<AtomClass> *> _steps;
  StepIndex _length;
  Channel<AtomClass> * const _channel;
  const StepIndex _pattern_capacity;
};

#endif /* Pattern_hpp */
