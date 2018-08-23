//
//  Workspace.hpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef Workspace_hpp
#define Workspace_hpp

#include <cstddef>

class Session;

class Workspace {
public:
  Workspace();
  Session *CurrentSession();
  virtual ~Workspace();
private:
  Workspace(const size_t &session_capacity);
  const size_t _session_capacity;
  Session *_current_session;
};

#endif /* Workspace_hpp */
