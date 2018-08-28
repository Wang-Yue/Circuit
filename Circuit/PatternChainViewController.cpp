//
//  PatternChainViewController.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/28/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "PatternChainViewController.hpp"
#include "Session.hpp"
#include "Channel.hpp"
#include "PatternChainRunner.hpp"
#include "CircuitView.hpp"
#include "Step.hpp"
#include "CircuitController.hpp"

static const Color kAvailableColor(0x0000003f);
static const Color kEnabledColor(0x000000ff);

PatternChainViewController::PatternChainViewController(CircuitController *parent) :
ScreenController(parent) {
  _pads = GetView()->GetRegularPads(0, kRegularPadCount);
  for (Pad *pad : _pads) {
    pad->SetDelegate(this);
  }
}
PatternChainViewController::~PatternChainViewController() {
  for (Pad *pad : _pads) {
    pad->SetDelegate(nullptr);
  }
}

void PatternChainViewController::Update() {
  for (Pad *pad : _pads) {
    PadIndex pad_index = pad->GetPadIndex();
    ChannelIndex channel_index = pad_index % 8 / 2;
    PatternIndex pattern_index = pad_index % 8 % 2 + pad_index / 8 * 2;
    PatternIndex current_pattern_index;
    if (channel_index < kSynthChannelsCapacity) {
      // A synth Channel.
      current_pattern_index = GetCurrentSynthPattern(channel_index)->GetPatternIndex();
    } else {
      // A sample Channel.
      channel_index = (channel_index - kSynthChannelsCapacity) * 2;
      current_pattern_index = GetCurrentSamplePattern(channel_index)->GetPatternIndex();
    }
    if (current_pattern_index == pattern_index) {
      pad->SetColor(kEnabledColor);
    } else {
      pad->SetColor(kAvailableColor);
    }
  }
}

void PatternChainViewController::UpdateRunningMode() {
  // no-op
}

void PatternChainViewController::UpdateEditingMode() {
  // no-op
}

void PatternChainViewController::Tap(Pad *pad) {
  PadIndex pad_index = pad->GetPadIndex();
  ChannelIndex channel_index = pad_index % 8 / 2;
  PatternIndex pattern_index = pad_index % 8 % 2 + pad_index / 8 * 2;
  if (channel_index < kSynthChannelsCapacity) {
    // A synth Channel.
    PatternChainRunner<Synth> * pattern_chain_runner = GetSynthPatternChainRunner(channel_index);
    Pattern<Synth> *pattern =
        GetCurrentSession()->GetSynthChannel(channel_index)->GetPattern(pattern_index);
    if (IsHoldingShift()) {
      pattern_chain_runner->AddPattern(pattern);
    } else {
      pattern_chain_runner->Reset(pattern);
      if (IsStopped()) {
        _parent->Restart();
      }
    }
  } else {
    channel_index = (channel_index - kSynthChannelsCapacity) * 2;
    ChannelIndex companion_channel_index = channel_index + 1;
    // A synth Channel.
    PatternChainRunner<Sample> * pattern_chain_runner = GetSamplePatternChainRunner(channel_index);
    PatternChainRunner<Sample> * companion_pattern_chain_runner =
        GetSamplePatternChainRunner(companion_channel_index);
    Pattern<Sample> *pattern =
        GetCurrentSession()->GetSampleChannel(channel_index)->GetPattern(pattern_index);
    Pattern<Sample> *companion_pattern = GetCurrentSession()->
        GetSampleChannel(companion_channel_index)->GetPattern(companion_channel_index);
    if (IsHoldingShift()) {
      pattern_chain_runner->AddPattern(pattern);
      companion_pattern_chain_runner->AddPattern(companion_pattern);
    } else {
      pattern_chain_runner->Reset(pattern);
      companion_pattern_chain_runner->Reset(companion_pattern);
      if (IsStopped()) {
        _parent->Restart();
      }
    }
  }
}
void PatternChainViewController::Release(Pad *) {
  // no-op.
}
