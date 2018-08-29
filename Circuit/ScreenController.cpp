//
//  ScreenController.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "ScreenController.hpp"
#include "CircuitController.hpp"
#include "CircuitView.hpp"
#include "Channel.hpp"
#include "Step.hpp"
#include "Sample.hpp"
#include "Synth.hpp"
#include "PatternChain.hpp"
#include "SessionRunner.hpp"
#include "ChannelRunner.hpp"

ScreenController::ScreenController(CircuitController *parent) : _parent(parent) {
}

ScreenController::~ScreenController() {
}

Pattern<Sample> * ScreenController::GetCurrentSamplePattern(const ChannelIndex &index) const {
  return GetSamplePatternChainRunner(index)->GetPattern();
}

Pattern<Synth> * ScreenController::GetCurrentSynthPattern(const ChannelIndex &index) const {
  return GetSynthPatternChainRunner(index)->GetPattern();
}

PatternChainRunner<Sample> *ScreenController::GetSamplePatternChainRunner(const ChannelIndex &index) const {
  return GetSampleChannelRunner(index)->GetPatternChainRunner();
}

PatternChainRunner<Synth> *ScreenController::GetSynthPatternChainRunner(const ChannelIndex &index) const {
  return GetSynthChannelRunner(index)->GetPatternChainRunner();
}

ChannelRunner<Sample> *ScreenController::GetSampleChannelRunner(const ChannelIndex &index) const {
  return GetSessionRunner()->GetSampleChannelRunner(index);
}
ChannelRunner<Synth> *ScreenController::GetSynthChannelRunner(const ChannelIndex &index) const {
  return GetSessionRunner()->GetSynthChannelRunner(index);
}

CircuitView *ScreenController::GetView() const {
  return _parent->GetView();
}

CircuitController *ScreenController::GetParent() const {
  return _parent;
}
Session *ScreenController::GetCurrentSession() const {
  return _parent->GetCurrentSession();
}

SessionRunner *ScreenController::GetSessionRunner() const  {
  return _parent->GetSessionRunner();
}

void ScreenController::SetMidiDelegate(MIDIDelegate *midi_delegate) {
  _parent->SetMidiDelegate(midi_delegate);
}

bool ScreenController::IsHoldingShift() const {
  return _parent->IsHoldingShift();
}

void ScreenController::HandleOctUp() {
  return;
}

void ScreenController::HandleOctDown() {
  return;
}

bool ScreenController::IsStopped() const {
  return _parent->IsStopped();
}

bool ScreenController::IsPlaying() const {
  return _parent->IsPlaying();
}

bool ScreenController::IsRecording() const {
  return _parent->IsRecording();
}

enum CircuitEditingMode ScreenController::GetEditingMode() const {
  return _parent->GetEditingMode();
}
