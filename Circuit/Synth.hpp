//
//  Synth.hpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef Synth_hpp
#define Synth_hpp

#include "Atom.hpp"

class Synth : public Atom {
public:
  Synth(const Note &note);
  Synth(const Note &note, const Gate &gate, const Velocity &velocity);

  Synth(const Synth &a);
  virtual ~Synth();
  Gate GetGate() const {
    return _gate;
  }
  void SetGate(const Gate &gate) {
    _gate = gate;
  }
  Note GetNote() const {
    return _note;
  }
  void SetNote(const Note &note) {
    _note = note;
  }

private:

  Gate _gate;
  Note _note;
};

#endif /* Synth_hpp */
