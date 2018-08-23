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

  Channel<Synth> *GetSynthChannel(const size_t &channel_index) const;
  size_t GetSynthChannelCount() const;
  Channel<Sample> *GetSampleChannel(const size_t &channel_index) const;
  size_t GetSampleChannelCount() const;

private:
  Session(const size_t &synth_channels_capacity, const size_t &sample_channels_capacity);
  std::vector<Channel<Synth> *> _synth_channels;
  std::vector<Channel<Sample> *> _sample_channels;
  Note _base_note;
  Degree _tonic_degree;
};

#endif /* Session_hpp */
