//
//  UIDefs.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/25/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef UIDefs_hpp
#define UIDefs_hpp

#include <cstdint>
#include <cstdlib>
#include <cstddef>

typedef uint8_t PadIndex;
static const PadIndex kPadsInRow = 8;

typedef uint8_t ColorBrightness;
typedef uint32_t ColorCode;

static const PadIndex kRegularPadCount = 32;

enum CircuitPad : PadIndex {
  PadRegular = 0,
  PadNote = kRegularPadCount,
  PadGate,
  PadVelocity,
  PadNudge,
  PadLength,
  PadScale,
  PadOctUp,
  PadOctDown,
  PadTempo,
  PadSwing,
  PadClear,
  PadDuplicate,
  PadSave,
  PadSessions,
  PadShift,
  PadPatterns,
  PadMixer,
  PadFX,
  PadRecord,
  PadPlay,
  PadSynth1,
  PadSynth2,
  PadDrum1,
  PadDrum2,
  PadDrum3,
  PadDrum4,
  PadUnknown,
};


enum CircuitRunningMode : uint8_t {
  CircuitStopMode,
  CircuitPlayingMode,
  CircuitRecordMode,
};

enum CircuitEditingMode : uint8_t {
  CircuitEditNoteMode,
  CircuitEditExpandNoteMode,
  CircuitEditGateMode,
  CircuitEditVelocityMode,
  CircuitEditNudgeMode,
  CircuitEditLengthMode,
};

enum CircuitViewMode : uint8_t {
  CircuitAtomSynth,
  CircuitAtomSample,
};

#endif /* UIDefs_hpp */
