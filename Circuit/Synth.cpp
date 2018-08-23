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
Atom(velocity), _note(note), _gate(gate) {
}

Synth::Synth(const Synth &a) : Atom(a), _gate(a._gate), _note(a._note) {
}

Synth::~Synth() {
}

