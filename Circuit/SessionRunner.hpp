//
//  SessionRunner.hpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef SessionRunner_hpp
#define SessionRunner_hpp

#include <vector>
#include "TypeDefs.hpp"

class Synth;
class Sample;
class Session;
template <typename AtomClass> class ChannelRunner;

class SessionRunner {
public:
  SessionRunner(Session *session);
  virtual ~SessionRunner();
  
  ChannelRunner<Synth> *GetSynthChannelRunner(const ChannelIndex &index) const;
  ChannelIndex GetSynthChannelRunnerCount() const;
  ChannelRunner<Sample> *GetSampleChannelRunner(const ChannelIndex &index) const;
  ChannelIndex GetSampleChannelRunnerCount() const;
  
  void TickMicrostep();
  void Restart();
private:
  Session *_session;
  std::vector<ChannelRunner<Synth> *> _synth_channel_runners;
  std::vector<ChannelRunner<Sample> *> _sample_channel_runners;
};

#endif /* SessionRunner_hpp */
