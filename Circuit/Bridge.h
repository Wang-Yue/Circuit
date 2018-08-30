//
//  Bridge.h
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef Bridge_h
#define Bridge_h

#include <stdint.h>

// Slave to call.
void InitializeCircuit(void);
// Thread safe.
void TapPad(uint8_t pad);
// Thread safe.
void ReleasePad(uint8_t pad);

// Slave to implement.
void WritePadColor(const uint32_t pad, const uint32_t color);
void WriteNovationPadColor(const uint32_t pad, const uint32_t color);

#endif /* Bridge_h */
