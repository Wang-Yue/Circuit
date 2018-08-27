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
void TapPad(uint8_t pad);
void ReleasePad(uint8_t pad);

// slave to implement.
void WritePadColor(const int &pad, const uint32_t &color);

#endif /* Bridge_hpp */
