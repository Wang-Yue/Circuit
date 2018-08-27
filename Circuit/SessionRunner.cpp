//
//  SessionRunner.cpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "SessionRunner.hpp"
#include "ChannelRunner.hpp"
#include "Session.hpp"
#include "Synth.hpp"
#include "Sample.hpp"

SessionRunner::SessionRunner(Session *session) :
_session(session) {
  ChannelIndex synth_channel_count = session->GetSynthChannelCount();
  for (int i = 0; i < synth_channel_count; ++i) {
    ChannelRunner<Synth> *runner = new ChannelRunner<Synth>(session->GetSynthChannel(i));
    _synth_channel_runners.push_back(runner);
  }
  ChannelIndex sample_channel_count = session->GetSampleChannelCount();
  for (int i = 0; i < sample_channel_count; ++i) {
    ChannelRunner<Sample> *runner = new ChannelRunner<Sample>(session->GetSampleChannel(i));
    _sample_channel_runners.push_back(runner);
  }
}

SessionRunner::~SessionRunner() {
  for (ChannelRunner<Synth> *runner: _synth_channel_runners) {
    delete runner;
  }
  for (ChannelRunner<Sample> *runner: _sample_channel_runners) {
    delete runner;
  }
}

void SessionRunner::TickMicrostep() {
  for (ChannelRunner<Synth> *runner: _synth_channel_runners) {
    runner->TickMicrostep();
  }
  for (ChannelRunner<Sample> *runner: _sample_channel_runners) {
    runner->TickMicrostep();
  }
}

ChannelRunner<Synth> *SessionRunner::GetSynthChannelRunner(const ChannelIndex &index) const {
  return _synth_channel_runners[index];
}

ChannelIndex SessionRunner::GetSynthChannelRunnerCount() const {
  return _synth_channel_runners.size();
}

ChannelRunner<Sample> *SessionRunner::GetSampleChannelRunner(const ChannelIndex &index) const {
  return _sample_channel_runners[index];
}

ChannelIndex SessionRunner::GetSampleChannelRunnerCount() const {
  return _sample_channel_runners.size();
}

