//
//  Atom.cpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "Atom.hpp"


Atom::Atom(): Atom(kDefaultVelocity) {
}

Atom::~Atom() {
  
}

Atom::Atom(const Atom &a) : _velocity(a._velocity) {
}

Atom::Atom(const Velocity &velocity) : _velocity(velocity) {
}
