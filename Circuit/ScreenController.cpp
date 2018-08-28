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
#include "SessionRunner.hpp"


ScreenController::ScreenController(CircuitController *parent) : _parent(parent) {
}

ScreenController::~ScreenController() {
}

Pattern<Sample> * ScreenController::GetCurrentSamplePattern(const ChannelIndex &index) {
  return GetSamplePatternChainRunner(index)->GetPattern();
}

Pattern<Synth> * ScreenController::GetCurrentSynthPattern(const ChannelIndex &index) {
  return GetSynthPatternChainRunner(index)->GetPattern();
}

PatternChainRunner<Sample> *ScreenController::GetSamplePatternChainRunner(const ChannelIndex &index) {
  return GetSampleChannelRunner(index)->GetPatternChainRunner();
}

PatternChainRunner<Synth> *ScreenController::GetSynthPatternChainRunner(const ChannelIndex &index) {
  return GetSynthChannelRunner(index)->GetPatternChainRunner();
}

ChannelRunner<Sample> *ScreenController::GetSampleChannelRunner(const ChannelIndex &index) {
  return GetSessionRunner()->GetSampleChannelRunner(index);
}
ChannelRunner<Synth> *ScreenController::GetSynthChannelRunner(const ChannelIndex &index) {
  return GetSessionRunner()->GetSynthChannelRunner(index);
}

CircuitView *ScreenController::GetView() {
  return _parent->GetView();
}

Session *ScreenController::GetCurrentSession() {
  return _parent->GetCurrentSession();
}

SessionRunner *ScreenController::GetSessionRunner() {
  return _parent->GetSessionRunner();
}

void ScreenController::SetMidiDelegate(MIDIDelegate *midi_delegate) {
  _parent->SetMidiDelegate(midi_delegate);
}

bool ScreenController::IsHoldingShift() const {
  return _parent->IsHoldingShift();
}

bool ScreenController::IsFixedVelocityMode() const {
  return _parent->IsFixedVelocityMode();
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
