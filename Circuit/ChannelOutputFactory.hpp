//
//  ChannelOutputFactory.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/26/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef ChannelOutputFactory_hpp
#define ChannelOutputFactory_hpp

#include "TypeDefs.hpp"
#include <vector>

class SynthChannelOutputInterface;
class SampleChannelOutputInterface;

class ChannelOutputFactory {
public:
  static ChannelOutputFactory& GetInstance();
  ChannelOutputFactory(ChannelOutputFactory const&) = delete;
  void operator=(ChannelOutputFactory const&) = delete;

  SynthChannelOutputInterface *GetSynthChannelOutput(const ChannelIndex &index);
  SampleChannelOutputInterface *GetSampleChannelOutput(const ChannelIndex &index);
private:
  ChannelOutputFactory();
  std::vector<SynthChannelOutputInterface *> _synth_output;
  std::vector<SampleChannelOutputInterface *> _sample_output;
};

#endif /* ChannelOutputFactory_hpp */
