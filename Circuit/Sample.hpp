//
//  Sample.hpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef Sample_hpp
#define Sample_hpp

#include "Atom.hpp"
#include "TypeDefs.hpp"

class Sample : public Atom {
public:
  Sample();
  Sample(const SampleIndex &_sample_index);
  Sample(const Sample &a);
  virtual ~Sample();
  bool IsSampleFlip() const;
  SampleIndex GetSampleIndex() const;
  void SetMicrostepTicks(const Microstep &microstep_ticks);
  Microstep GetMicrostepTicks() const;
  // Set the sample index will convert the sample to a sample flip.
  void SetSampleIndex(const SampleIndex &index);

private:
  bool _is_sample_flip;
  SampleIndex _sample_index;
  Microstep _microstep_ticks;
};

#endif /* Sample_hpp */
