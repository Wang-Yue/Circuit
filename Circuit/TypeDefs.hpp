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

typedef uint8_t Swing;
static const Swing kDefaultSwing = 50;

typedef uint8_t Velocity;
static const Velocity kDefaultVelocity = 96;
static const Velocity kDiscreteVelocityValues = 128;

typedef uint8_t Gate;
static const Gate kDiscreteGateValues = 96;
static const Gate kDefaultGate = 6;

typedef uint8_t Volume;
static const Volume kDefaultVolume = 127;
static const Volume kMaxVolume = 127;

typedef uint8_t Pan;
static const Volume kDefaultPan = 63;

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

typedef uint8_t Control;
typedef uint8_t CC;

static const Control kControlCapacity = 8;
static const CC kMaxCC = 127;


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
static const Octave kMaxOctave = 6;
static const Octave kMinOctave = - kBaseOctave - 1;
static const Degree kMIDIDegreeRange = 128;

struct Note {
  Scale scale;
  Octave octave;
  Degree degree;
};



#endif /* TypeDefs_hpp */
