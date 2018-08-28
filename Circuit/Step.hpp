//
//  Step.hpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef Step_hpp
#define Step_hpp

#include <vector>
#include <map>

#include "TypeDefs.hpp"
#include "Sample.hpp"
#include "Synth.hpp"
#include "AtomPacket.hpp"
#include "Scale.hpp"

class Atom;
template <typename AtomClass> class AtomPacket;
template <typename AtomClass> class Pattern;

template <typename AtomClass = Atom>
class StepBase {
public:
  virtual ~StepBase()  {
    delete _atom_packet;
  }

  bool AddAtom(AtomClass *atom)  {
    return _atom_packet->AddAtom(atom);
  }
  
  void RemoveAllAtoms(){
    _atom_packet->RemoveAll();
  }
  
  void RemoveAtom(AtomClass *atom){
    _atom_packet->RemoveAtom(atom);
  }

  std::vector<AtomClass *> GetAtoms() const  {
    return _atom_packet->GetAtoms();
  }
  
  void RecordAutomation(const AutomationTrait &trait, const Automation &automation)  {
    _automations[trait] = automation;
  }
  void ClearAutomation(const AutomationTrait &trait)  {
    _automations.erase(trait);
  }
  bool HasAutomation(const AutomationTrait &trait) const  {
    return _automations.count(trait) != 0;
  }
  
  // If no automation is available, return 0;
  Automation GetAutomation(const AutomationTrait &trait) const  {
    if (_automations.count(trait)) {
      return _automations.at(trait);
    } else {
      return 0;
    }
  }
  
  Velocity GetVelocity() const {
    Velocity velocity = 0;
    std::vector<AtomClass *> atoms = GetAtoms();
    for (AtomClass *atom : atoms) {
      Velocity atom_velocity = atom->GetVelocity();
      if (atom_velocity > velocity) {
        velocity = atom_velocity;
      }
    }
    return velocity;
  }
  void SetVelocity(const Velocity &velocity) {
    std::vector<AtomClass *> atoms = GetAtoms();
    for (AtomClass *atom : atoms) {
      atom->SetVelocity(velocity);
    }
  }

protected:
  StepBase(Pattern<AtomClass> * const parent, const size_t &polyphony_capacity) :
  _polyphony_capacity(polyphony_capacity), _parent(parent) {
    _atom_packet = new AtomPacket<AtomClass>();
  }
  
private:
  const size_t _polyphony_capacity;
  Pattern<AtomClass> * const _parent;
  AtomPacket<AtomClass> *_atom_packet;
  std::map<AutomationTrait, Automation> _automations;
};


template <typename AtomClass = Atom>
class Step : public StepBase<AtomClass> {
};

template <>
class Step<Sample> : public StepBase<Sample> {
public:
  Step(Pattern<Sample> * const parent) : StepBase<Sample>(parent, kSamplePolyphonyCapacity) {
  }

  void SetMicrostepTicks(const Microstep &microstep_ticks)  {
    for (Sample *sample : GetAtoms()) {
      sample->SetMicrostepTicks(microstep_ticks);
    }
  }

  Microstep GetMicrostepTicks() const  {
    Microstep ticks = 0;
    for (Sample *sample : GetAtoms()) {
      ticks |= sample->GetMicrostepTicks();
    }
    return ticks;
  }
private:
};

template <>
class Step<Synth> : public StepBase<Synth>{
public:
  Step(Pattern<Synth> * const parent) : StepBase<Synth>(parent, kSynthPolyphonyCapacity)  {
  }
  void SetMicrostepDelay(const Microstep &microstep_delay)  {
    std::vector<Synth *> atoms = GetAtoms();
    for (Synth *atom : atoms) {
      atom->SetMicrostepDelay(microstep_delay);
    }
  }
  
  Microstep GetMicrostepDelay() const  {
    std::vector<Synth *> atoms = GetAtoms();
    return atoms[0]->GetMicrostepDelay();
  }
  
  void SetTie(const bool &tie) {
    std::vector<Synth *> atoms = GetAtoms();
    for (Synth *atom : atoms) {
      atom->SetTie(tie);
    }
  }
  
  bool GetTie() const {
    std::vector<Synth *> atoms = GetAtoms();
    for (Synth *atom : atoms) {
      if (atom->GetTie()) {
        return true;
      }
    }
    return false;
  }
  
  Gate GetGate() const  {
    Gate gate = 0;
    std::vector<Synth *> atoms = GetAtoms();
    for (Synth *atom : atoms) {
      Gate atom_gate = atom->GetGate();
      if (atom_gate > gate) {
        gate = atom_gate;
      }
    }
    return gate;
  }
  
  void SetGate(const Gate &gate)  {
    std::vector<Synth *> atoms = GetAtoms();
    for (Synth *atom : atoms) {
      atom->SetGate(gate);
    }
  }

  Synth *AddNote(const Note &note)  {
    std::vector<Synth *> atoms = GetAtoms();
    Gate gate = kDefaultGate;
    Velocity velocity = kDefaultVelocity;
    if (atoms.size() != 0) {
      // Set the gate/velocity to be the first atom's gate/velocity.
      Synth *atom = atoms[0];
      gate = atom->GetGate();
      velocity = atom->GetVelocity();
    }
    Synth *synth = new Synth(note, gate, velocity);
    if (AddAtom(synth)) {
      return synth;
    } else {
      delete synth;
      return nullptr;
    }
  }

  bool HasNote(const Note &note)  {
    std::vector<Synth *> atoms = GetAtoms();
    for (Synth *synth : atoms) {
      Note n = synth->GetNote();
      if (NoteEquals(note, n)) {
        return true;
      }
    }
    return false;
  }
  
  // Return false if no note found.
  bool RemoveNote(const Note &note) {
    bool result = false;
    std::vector<Synth *> atoms = GetAtoms();
    for (Synth *synth : atoms) {
      Note n = synth->GetNote();
      if (NoteEquals(note, n)) {
        RemoveAtom(synth);
        result = true;
      }
    }
    return result;
  }
  
  std::vector<Note> GetNotes() const  {
    std::vector<Note> notes;
    std::vector<Synth *> atoms = GetAtoms();
    for (Synth *synth : atoms) {
      notes.push_back(synth->GetNote());
    }
    return notes;
  }
  
  void OctUp() {
    std::vector<Synth *> atoms = GetAtoms();
    for (Synth *synth : atoms) {
      synth->OctUp();
    }
  }
  
  void OctDown() {
    std::vector<Synth *> atoms = GetAtoms();
    for (Synth *synth : atoms) {
      synth->OctDown();
    }
  }
};

#endif /* Step_hpp */
