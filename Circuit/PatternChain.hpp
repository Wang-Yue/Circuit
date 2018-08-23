//
//  PatternChain.hpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef PatternChain_hpp
#define PatternChain_hpp

#include <vector>

static const size_t kPatternChainCapacity = 32;


class Atom;
template <typename AtomClass> class Step;
template <typename AtomClass> class Pattern;

template <typename AtomClass = Atom>
class PatternChain {
public:
  PatternChain(Pattern<AtomClass> *initial_pattern) :
  PatternChain(initial_pattern, kPatternChainCapacity) {
  }
  
  virtual ~PatternChain()  {
    // no-op
  }

  void AddPattern(Pattern<AtomClass> *pattern)  {
    if (_patterns.size() >= _pattern_chain_capacity) {
      throw std::overflow_error("Pattern chain size reached");
    }
    _patterns.push_back(pattern);
  }

  void Reset(Pattern<AtomClass> *pattern) {
    AddPattern(pattern);
  }

  size_t GetPatternCount() const  {
    return _patterns.size();
  }


  // NotNull
  Pattern<AtomClass> *GetPattern(const size_t &pattern_index) const {
    if (pattern_index >= _patterns.size()) {
      throw std::out_of_range("Exceed maximum pattern steps.");
    }
    return _patterns[pattern_index];
  }
  
  // NotNull
  Step<AtomClass> *GetStep(const size_t &pattern_index, const StepIndex &step_index) const  {
    Pattern<AtomClass> *pattern = GetPattern(pattern_index);
    return pattern->GetStep(step_index);
  }

private:
  PatternChain(Pattern<AtomClass> *initial_pattern, const size_t &pattern_chain_capacity) :
  _pattern_chain_capacity(pattern_chain_capacity) {
    _patterns.reserve(pattern_chain_capacity);
    Reset(initial_pattern);
  }
  std::vector<Pattern<AtomClass> *>  _patterns;
  const size_t _pattern_chain_capacity;
};
#endif /* PatternChain_hpp */
