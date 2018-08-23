//
//  Session.cpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "Session.hpp"
#include "Channel.hpp"
#include "PatternChain.hpp"
#include "Step.hpp"
#include "Atom.hpp"
#include "Sample.hpp"

static const size_t kSynthChannelsCapacity = 2;
static const size_t kSampleChannelsCapacity = 4;

Session::Session() : Session(kSynthChannelsCapacity, kSampleChannelsCapacity){
}

Session::Session(const size_t &synth_channels_capacity,
                 const size_t &sample_channels_capacity){
  _tonic_degree = 0;
  _synth_channels.reserve(synth_channels_capacity);
  _sample_channels.reserve(sample_channels_capacity);
  for (size_t i = 0; i < synth_channels_capacity; ++i) {
    Channel<Synth> *channel = new Channel<Synth>(this);
    channel->InitializeChannel();
    _synth_channels.push_back(channel);
  }
  for (size_t i = 0; i < sample_channels_capacity; ++i) {
    Channel<Sample> *channel = new Channel<Sample>(this);
    channel->InitializeChannel();
    _sample_channels.push_back(channel);
  }
  _base_note.scale = ScaleNaturalMinor;
  _base_note.octave = 0;
  _base_note.degree = 0;
}

Session::~Session() {
  for (size_t i = 0; i < _synth_channels.size(); ++i) {
    Channel<Synth> *c = _synth_channels[i];
    delete c;
  }
  for (size_t i = 0; i < _sample_channels.size(); ++i) {
    Channel<Sample> *c = _sample_channels[i];
    delete c;
  }
}

void Session::SetTonicDegree(const Degree &tonic_degree) {
  _tonic_degree = tonic_degree;
}

const Degree Session::GetTonicDegree() const {
  return _tonic_degree;
}


void Session::SetBaseNote(const Note &base_note) {
  _base_note = base_note;
}

const Note Session::GetBaseNote() const {
  return _base_note;
}


Channel<Synth> *Session::GetSynthChannel(const size_t &channel_index) const {
  return _synth_channels[channel_index];
}

size_t Session::GetSynthChannelCount() const {
  return _synth_channels.size();
}

Channel<Sample> *Session::GetSampleChannel(const size_t &channel_index) const {
  return _sample_channels[channel_index];
}

size_t Session::GetSampleChannelCount() const {
  return _sample_channels.size();
}

