//
//  Sample.cpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "Sample.hpp"

Sample::Sample() :
Atom(), _sample_index(0), _is_sample_flip(false), _microstep_ticks(1 << 0) {
  
}

Sample::Sample(const SampleIndex &_sample_index) :
Atom(), _sample_index(_sample_index), _is_sample_flip(true), _microstep_ticks(1 << 0) {
}

Sample::~Sample() {
}

Sample::Sample(const Sample &a) :
Atom(a), _sample_index(a._sample_index), _is_sample_flip(a._is_sample_flip), _microstep_ticks(a._microstep_ticks) {
}

bool Sample::IsSampleFlip() const {
  return _is_sample_flip;
}

SampleIndex Sample::GetSampleIndex() const {
  return _sample_index;
}

void Sample::SetSampleIndex(const SampleIndex &index) {
  _sample_index = index;
  _is_sample_flip = true;
}

void Sample::SetMicrostepTicks(const Microstep &microstep_ticks)  {
  _microstep_ticks = microstep_ticks;
}

Microstep Sample::GetMicrostepTicks() const {
  return _microstep_ticks;
}
