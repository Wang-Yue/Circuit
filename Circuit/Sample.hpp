//
//  Sample.hpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef Sample_hpp
#define Sample_hpp

#include "Atom.hpp"
#include "TypeDefs.hpp"

class Sample : public Atom {
public:
  Sample();
  Sample(const Sample &a);
  virtual ~Sample();
  
private:
  SampleIndex _sample_index;
};

#endif /* Sample_hpp */
