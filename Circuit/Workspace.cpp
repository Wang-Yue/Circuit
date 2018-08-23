//
//  Workspace.cpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "Workspace.hpp"

#include "Session.hpp"

static const size_t kSessionCapacity = 32;

Workspace::Workspace() : Workspace(kSessionCapacity) {
}

Workspace::Workspace(const size_t &session_capacity) : _session_capacity(session_capacity) {
  Session *session = new Session();
  _current_session = session;
}

Workspace::~Workspace() {
  delete _current_session;
}

Session *Workspace::CurrentSession() {
  return _current_session;
}


