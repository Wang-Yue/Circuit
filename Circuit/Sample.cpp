//
//  Sample.cpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "Sample.hpp"

Sample::Sample() : Atom(), _sample_index(0) {
}

Sample::~Sample() {
}

Sample::Sample(const Sample &a) : Atom(a) {
}
