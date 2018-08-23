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

  Pattern(Channel<AtomClass> * const channel) : Pattern(kStepCapacity, channel) {
  }
  virtual ~Pattern()  {
    for (StepIndex i = 0; i < _steps.size(); ++i) {
      Step<AtomClass> *step = _steps[i];
      delete step;
    }
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
  
  void ClearAutomation(const AutomationTrait &trait)  {
    for (Step<AtomClass> *step : _steps) {
      step->ClearAutomation(trait);
    }
  }


private:
  Pattern(const StepIndex &pattern_capacity, Channel<AtomClass> * const channel) :
  _pattern_capacity(pattern_capacity), _channel(channel) {
    _steps.reserve(pattern_capacity);
    for (StepIndex i = 0; i < pattern_capacity; ++i) {
      Step<AtomClass> *step = new Step<AtomClass>(this);
      _steps.push_back(step);
    }
    SetLength(kStepCapacity);
  }
  
  std::vector<Step<AtomClass> *> _steps;
  StepIndex _length;
  Channel<AtomClass> * const _channel;
  const StepIndex _pattern_capacity;
};

#endif /* Pattern_hpp */
