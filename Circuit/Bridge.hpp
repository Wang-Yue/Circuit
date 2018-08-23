//
//  Bridge.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef Bridge_hpp
#define Bridge_hpp


#include <cstdint>
#include <cstdlib>
#include <cstddef>


void InitializeCircuit();
void PressKey(int key);
void ReleaseKey(int key);
uint32_t GetColor(int item);
void Tick();

void Beep();

#endif /* Bridge_hpp */
