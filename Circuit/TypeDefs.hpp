//
//  TypeDefs.hpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef TypeDefs_hpp
#define TypeDefs_hpp

#include <cstdint>
#include <cstdlib>
#include <cstddef>

typedef uint8_t BPM;
static const BPM kDefaultBPM = 90;

typedef uint8_t Velocity;
static const Velocity kDefaultVelocity = 96;
static const Velocity kDiscreteVelocityValues = 128;

typedef uint8_t Gate;
static const Gate kDiscreteGateValues = 128;
static const Gate kDefaultGate = 8;

typedef uint8_t Volume;
static const Volume kDefaultVolume = 127;

typedef uint8_t Microstep;
static const Microstep kMicrosteps = 6;

typedef uint8_t SynthIndex;
typedef uint8_t SampleIndex;

typedef uint8_t StepIndex;
typedef uint8_t PatternIndex;
typedef uint8_t ChannelIndex;

static const ChannelIndex kSynthChannelsCapacity = 2;
static const ChannelIndex kSampleChannelsCapacity = 4;

static const size_t kSynthPolyphonyCapacity = 6;
static const size_t kSamplePolyphonyCapacity = 1;

static const StepIndex kStepCapacity = 16;
static const PatternIndex kPatternCapacity = 8;

typedef int8_t Automation;

enum AutomationTrait : uint8_t {
  AutomationTraitVolume = 0,
  AutomationTraitFX,
  AutomationTraitPan,
  AutomationTraitKnob = 1 << 4,
};

enum KnobIndex : uint8_t {
  KnobIndex0 = 0,
  KnobIndex1,
  KnobIndex2,
  KnobIndex3,
  KnobIndex4,
  KnobIndex5,
  KnobIndex6,
  KnobIndex7
};

enum Scale : uint8_t {
  ScaleNaturalMinor = 0,
  ScaleMajor,
  ScaleDorian,
  ScalePhrygian,
  ScaleMixolydian,
  ScaleMelodicMinor,
  ScaleHarmonicMinor,
  ScaleBebopDorian,
  ScaleBlues,
  ScaleMinorPentatonic,
  ScaleHungarianMinor,
  ScaleUkranianDorian,
  ScaleMarva,
  ScaleTodi,
  ScaleWholeTone,
  ScaleChromatic,
  ScaleUnknown,
};

typedef int8_t MIDINote;
typedef uint8_t Degree;
typedef int8_t Octave;

static const Degree kOctaveDegree = 12;
static const Octave kBaseOctave = 5; // C5 have octave 0.
static const Degree kMIDIDegreeRange = 128;

struct Note {
  Scale scale;
  Octave octave;
  Degree degree;
};



#endif /* TypeDefs_hpp */
