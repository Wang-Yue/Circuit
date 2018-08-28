//
//  SampleExpandNoteViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/27/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef SampleExpandNoteViewController_hpp
#define SampleExpandNoteViewController_hpp

#include <map>
#include <vector>
#include "TypeDefs.hpp"
#include "Pad.hpp"

class SampleExpandNoteViewControllerDelegate {
public:
  virtual void TapChannel(const ChannelIndex &channel_index) = 0;
  virtual void ReleaseChannel(const ChannelIndex &channel_index) = 0;
};

class SampleExpandNoteViewController : public PadDelegate {
public:
  SampleExpandNoteViewController(const std::vector<Pad *> &pads,
                               SampleExpandNoteViewControllerDelegate *delegate,
                                 const ChannelIndex &channel_count);
  virtual ~SampleExpandNoteViewController();
  void SetHightlightChannels(const std::vector<bool> &channels);
  void Update();
  // PadDelegate.
  virtual void Tap(Pad *pad) override;
  virtual void Release(Pad *pad) override;
  
private:
  std::vector<bool> _highlight_channels;
  SampleExpandNoteViewControllerDelegate *_delegate;
  std::map<PadIndex, ChannelIndex> _pad_channel_mapping;
  std::map<ChannelIndex, Pad *> _channel_pad_mapping;
};

#endif /* SampleExpandNoteViewController_hpp */
