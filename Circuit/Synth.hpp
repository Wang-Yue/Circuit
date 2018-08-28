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
  Gate GetGate() const;
  void SetGate(const Gate &gate);
  Note GetNote() const;
  void SetNote(const Note &note);
  void OctUp();
  void OctDown();
  void SetMicrostepDelay(const Microstep &microstep_delay);
  Microstep GetMicrostepDelay() const;
  void SetTie(const bool &tie);
  bool GetTie() const;
private:
  Gate _gate;
  Note _note;
  Microstep _microstep_delay;
  bool _tie;
};

#endif /* Synth_hpp */
