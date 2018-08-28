//
//  SampleExpandNoteViewController.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/27/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "SampleExpandNoteViewController.hpp"


static const Color kHighlightColor(0x0000007f);
static const Color kEnabledColor(0x0000003f);
static const Color kDisabledColor(0x00000000);


SampleExpandNoteViewController::SampleExpandNoteViewController(const std::vector<Pad *> &pads,
                                                               SampleExpandNoteViewControllerDelegate *delegate,
                                                               const ChannelIndex &channel_count) :
_delegate(delegate) {
  assert(pads.size() == kRegularPadCount);
  for (PadIndex i = 0; i < pads.size() - channel_count; ++i) {
    pads[i]->SetDelegate(nullptr);
    pads[i]->SetColor(kDisabledColor);
  }
  for (ChannelIndex channel_index = 0; channel_index < channel_count; ++channel_index) {
    PadIndex pad_index = pads.size() - channel_count + channel_index;
    Pad *pad = pads[pad_index];
    _pad_channel_mapping[pad_index] = channel_index;
    _channel_pad_mapping[channel_index] = pad;
    pad->SetDelegate(this);
  }
}

SampleExpandNoteViewController::~SampleExpandNoteViewController() {
  for (std::pair<const ChannelIndex, Pad *> &p : _channel_pad_mapping) {
    p.second->SetDelegate(nullptr);
  }
}

void SampleExpandNoteViewController::Tap(Pad *pad) {
  if (_delegate) {
    PadIndex pad_index = pad->GetPadIndex();
    ChannelIndex channel_index = _pad_channel_mapping[pad_index];
    _delegate->TapChannel(channel_index);
  }
}

void SampleExpandNoteViewController::Release(Pad *pad) {
  if (_delegate) {
    PadIndex pad_index = pad->GetPadIndex();
    ChannelIndex channel_index = _pad_channel_mapping[pad_index];
    _delegate->ReleaseChannel(channel_index);
  }
}

void SampleExpandNoteViewController::SetHightlightChannels(const std::vector<bool> &channels) {
  _highlight_channels = channels;
}

void SampleExpandNoteViewController::Update() {
  for (std::pair<const ChannelIndex, Pad *> &p : _channel_pad_mapping) {
    Pad *pad = p.second;
    const ChannelIndex index = p.first;
    if (_highlight_channels[index] == true) {
      pad->SetColor(kHighlightColor);
    } else {
      pad->SetColor(kEnabledColor);
    }
  }
}
