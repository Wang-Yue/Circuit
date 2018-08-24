//
//  Session.hpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef Session_hpp
#define Session_hpp

#include <vector>
#include "TypeDefs.hpp"

template <typename AtomClass> class Channel;
class Synth;
class Sample;

class Session {
public:
  Session();
  virtual ~Session();
  
  void SetTonicDegree(const Degree &tonic_degree);
  const Degree GetTonicDegree() const;
  void SetBaseNote(const Note &base_note);
  const Note GetBaseNote() const;

  Channel<Synth> *GetSynthChannel(const ChannelIndex &channel_index) const;
  ChannelIndex GetSynthChannelCount() const;
  Channel<Sample> *GetSampleChannel(const ChannelIndex &channel_index) const;
  ChannelIndex GetSampleChannelCount() const;

private:
  Session(const ChannelIndex &synth_channels_capacity, const ChannelIndex &sample_channels_capacity);
  std::vector<Channel<Synth> *> _synth_channels;
  std::vector<Channel<Sample> *> _sample_channels;
  Note _base_note;
  Degree _tonic_degree;
};

#endif /* Session_hpp */
