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

  void AddAtom(AtomClass *atom)  {
    _atom_packet->AddAtom(atom);
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
    SetMicrostepTicks(1 << 0);
  }
  
  void SetMicrostepTicks(const Microstep &microstep_ticks)  {
    _microstep_ticks = microstep_ticks;
  }

  Microstep GetMicrostepTicks() const  {
    return _microstep_ticks;
  }
private:
  Microstep _microstep_ticks;
};

template <>
class Step<Synth> : public StepBase<Synth>{
public:
  Step(Pattern<Synth> * const parent) : StepBase<Synth>(parent, kSynthPolyphonyCapacity)  {
    SetMicrostepDelay(0);
    SetIsSampleFlip(false);
  }
  void SetMicrostepDelay(const Microstep &microstep_delay)  {
    _microstep_delay = microstep_delay;
  }
  
  Microstep GetMicrostepDelay() const  {
    return _microstep_delay;
  }
  
  void SetIsSampleFlip(const bool &is_sample_flip) {
    _is_sample_flip = is_sample_flip;
  }
  
  bool GetIsSampleFlip() const {
    return _is_sample_flip;
  }
  
  void SetIsTie(const bool &tie) {
    _is_tie = tie;
  }
  
  bool GetIsTie() const {
    return _is_tie;
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
    AddAtom(synth);
    return synth;
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
private:
  Microstep _microstep_delay;
  bool _is_sample_flip;
  bool _is_tie;
};

#endif /* Step_hpp */
