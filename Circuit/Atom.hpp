//
//  Atom.hpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef Atom_hpp
#define Atom_hpp

#include "TypeDefs.hpp"

class Atom {
public:
  Atom();
  Atom(const Atom &a);
  virtual ~Atom();

  Velocity GetVelocity() const {
    return _velocity;
  }
  void SetVelocity(const Velocity &velocity) {
    _velocity = velocity;
  }
protected:
  Atom( const Velocity &velocity);
private:
  Velocity _velocity;
};
#endif /* Atom_hpp */
