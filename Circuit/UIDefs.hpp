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
  PadRecord = kRegularPadCount,
  PadPlay,
  PadSynth1,
  PadSynth2,
  PadDrum1,
  PadDrum2,
  PadDrum3,
  PadDrum4,
  PadOctUp,
  PadOctDown,
  PadShift,
  PadNote,
  PadGate,
  PadVelocity,
  PadNudge,
  PadLength,
  PadPatterns,
  PadScale,
  PadTempo,
  PadSwing,
  PadClear,
  PadDuplicate,
  PadSave,
  PadSessions,
  PadMixer,
  PadFX,
  PadUnknown,
};


typedef uint8_t KnobIndex;
static const KnobIndex kKnobCount = 8;

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
  CircuitEditSynthMicrostepDelayMode,
  // This is never set to the circuit controller. Instead, it will be returned if it's in the
  // CircuitSettingPatchMode. The reason to that is we need to use the SynthViewController and
  // SampleViewController to handle taps. When user exit this mode, we want to revert to the
  // original editing mode.
  CircuitEditPatchMode,
};

enum CircuitSettingMode : uint8_t {
  CircuitSetRegularMode,
  CircuitSetPatchMode,
  CircuitSetTempoMode,
  CircuitSetSwingMode,
  CircuitSetFxMode,
  CircuitSetPatternChainMode,
  CircuitSetMixerMode,
  CircuitSetPanMode,
  CircuitSetScaleMode,
};

enum CircuitViewMode : uint8_t {
  CircuitAtomSynth,
  CircuitAtomSample,
};

#endif /* UIDefs_hpp */
