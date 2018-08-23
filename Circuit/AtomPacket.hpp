//
//  AtomPacket.hpp
//  Circuit
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef AtomPacket_hpp
#define AtomPacket_hpp

#include <vector>
#include <map>

#include "TypeDefs.hpp"

#include "Atom.hpp"
#include "Sample.hpp"
#include "Synth.hpp"

class Atom;

template <typename AtomClass>
class BaseAtomPacket {
public:
  BaseAtomPacket() = delete;
  
  virtual ~BaseAtomPacket()  {
    RemoveAll();
  }

  virtual bool AddAtom(AtomClass *atom) {
    if (_polyphony_capacity == _atoms.size()) {
      return false;
    }
    _atoms.push_back(atom);
    return true;
  }
  
  virtual bool RemoveAtom(AtomClass *atom)  {
    for (typename std::vector<AtomClass *>::iterator it = _atoms.begin(); it != _atoms.end(); ) {
      if (*it == atom) {
        it = _atoms.erase(it);
        return true;
      } else {
        ++it;
      }
    }
    return false;
  }
  
  void RemoveAll()  {
    for (AtomClass *atom : _atoms) {
      delete atom;
    }
    _atoms.clear();
  }

  std::vector<AtomClass *> GetAtoms() const  {
    return _atoms;
  }
protected:
  std::vector<AtomClass *> _atoms;
  
  BaseAtomPacket(const size_t &polyphony_capacity) :
  _polyphony_capacity(polyphony_capacity) {
  }
private:
  const size_t _polyphony_capacity;
};


template <typename AtomClass>
class AtomPacket : BaseAtomPacket<AtomClass> {
public:
  AtomPacket() = delete;
};

template <>
class AtomPacket<Sample> : public BaseAtomPacket<Sample> {
public:
  AtomPacket() : BaseAtomPacket<Sample>(kSamplePolyphonyCapacity) {
  }
};

template <>
class AtomPacket<Synth> : public BaseAtomPacket<Synth> {
public:
  AtomPacket() : BaseAtomPacket<Synth>(kSynthPolyphonyCapacity) {
  }
};

#endif /* AtomPacket_hpp */
