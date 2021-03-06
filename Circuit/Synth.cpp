//
//  Synth.cpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "Synth.hpp"


Synth::Synth(const Note &note): Synth(note, kDefaultGate, kDefaultVelocity) {
}

Synth::Synth(const Note &note, const Gate &gate, const Velocity &velocity) :
Atom(velocity), _note(note), _gate(gate),  _microstep_delay(0) {
}

Synth::Synth(const Synth &a) :
Atom(a), _gate(a._gate), _note(a._note),  _microstep_delay(0) {
}

Synth::~Synth() {
}

Gate Synth::GetGate() const {
  return _gate;
}

void Synth::SetGate(const Gate &gate) {
  _gate = gate;
}

Note Synth::GetNote() const {
  return _note;
}

void Synth::SetNote(const Note &note) {
  _note = note;
}

void Synth::OctUp() {
  ++_note.octave;
  if (_note.octave > kMaxOctave) {
    _note.octave = kMaxOctave;
  }
}

void Synth::OctDown() {
  --_note.octave;
  if (_note.octave < kMinOctave) {
    _note.octave = kMinOctave;
  }
}

void Synth::SetMicrostepDelay(const Microstep &microstep_delay)  {
  _microstep_delay = microstep_delay;
}

Microstep Synth::GetMicrostepDelay() const  {
  return _microstep_delay;
}

void Synth::SetTie(const bool &tie) {
  _tie = tie;
}

bool Synth::GetTie() const {
  return _tie;
}
