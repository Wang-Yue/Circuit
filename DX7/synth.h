/*
 * Copyright 2012 Google Inc.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __SYNTH_H
#define __SYNTH_H

// This may not be present on MSVC.
// See http://stackoverflow.com/questions/126279/c99-stdint-h-header-and-ms-visual-studio
#include <stdint.h>

// TODO(raph): move from fixed to variable N
#define LG_N 6
#define N (1 << LG_N)

#if defined(__APPLE__)
#include <libkern/OSAtomic.h>
#define SynthMemoryBarrier() OSMemoryBarrier()
#elif defined(__GNUC__)
#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 1)
#define SynthMemoryBarrier() __sync_synchronize()
#endif
#endif

// #undef SynthMemoryBarrier()

#ifndef SynthMemoryBarrier
#warning Memory barrier is not enabled
#define SynthMemoryBarrier()
#endif

template<typename T>
inline static T min(const T& a, const T& b) {
    return a < b ? a : b;
}

template<typename T>
inline static T max(const T& a, const T& b) {
    return a > b ? a : b;
}

#ifdef __aarch64__
#define HAVE_NEON_INTRINSICS
#include <arm_neon.h>
static inline bool hasNeon() {
  return true;
}
#elif __ARM_NEON__
#define HAVE_NEON_INTRINSICS
#include <arm_neon.h>
// TODO: get this working (although maybe we don't even care about non-NEON ARM any more)
//#include <cpu-features.h>

static inline bool hasNeon() {
  return true;
  //return (android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
}
#else
static inline bool hasNeon() {
  return false;
}
#endif

#endif  // __SYNTH_H
