//
//  PatternChainRunner.hpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef PatternChainRunner_hpp
#define PatternChainRunner_hpp

#include "TypeDefs.hpp"
#include "PatternChain.hpp"
#include "Pattern.hpp"

template <typename AtomClass> class Step;
class Atom;

template <typename AtomClass = Atom>
class PatternChainRunner {
public:
  PatternChainRunner(PatternChain<AtomClass> *pattern_chain)  :
  _pattern_chain(pattern_chain), _pattern_counter(0), _step_counter(0), _remaining_pattern(nullptr) {
  }

  ~PatternChainRunner(){
  }
  
  PatternChain<AtomClass> *GetPatternChain() const  {
    return _pattern_chain;
  }


  bool AddPattern(Pattern<AtomClass> *pattern)  {
    return _pattern_chain->AddPattern(pattern);
  }

  void Reset(Pattern<AtomClass> *pattern)  {
    _remaining_pattern = GetPattern();
    _pattern_chain->Reset(pattern);
  }


  // NotNull
  Pattern<AtomClass> *GetPattern() const  {
    if (_remaining_pattern) {
      return _remaining_pattern;
    }
    return _pattern_chain->GetPattern(GetPatternCounter());
  }

  // NotNull.
  Step<AtomClass> *GetStep() const {
    return GetPattern()->GetStep(GetStepCounter());
  }

  void TickStep()  {
    if (_step_counter < GetPattern()->GetLength() - 1) {
      ++_step_counter;
      return;
    }
    _step_counter = 0;
    if (_remaining_pattern) {
      _remaining_pattern = nullptr;
      _pattern_counter = 0;
    }
    if (_pattern_counter < GetPatternChain()->GetPatternCount() - 1) {
      ++_pattern_counter;
      return;
    }
    _pattern_counter = 0;
  }

  void AddAtom(AtomClass *atom)  {
    Step<AtomClass> *step = GetStep();
    step->AddAtom(atom);
  }
  
  PatternIndex GetPatternCounter() const  {
    return _pattern_counter;
  }
  StepIndex GetStepCounter() const  {
    return _step_counter;
  }
private:
  PatternChain<AtomClass> *_pattern_chain;
  Pattern<AtomClass> *_remaining_pattern;
  PatternIndex _pattern_counter;
  StepIndex _step_counter;
};

#endif /* PatternChainRunner_hpp */
