//
//  Scale.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "Scale.hpp"

#include <vector>

static const std::vector<Degree> natural_minor =    {0,2,3,5,7,8,10};
static const std::vector<Degree> major =            {0,2,4,5,7,9,11};
static const std::vector<Degree> dorian =           {0,2,3,5,7,9,10};
static const std::vector<Degree> phrygian =         {0,1,3,5,7,8,10};
static const std::vector<Degree> mixolydian =       {0,2,4,5,7,9,10};
static const std::vector<Degree> melodic_minor =    {0,2,3,5,7,9,11};
static const std::vector<Degree> harmonic_minor =   {0,2,3,5,7,8,11};
static const std::vector<Degree> bebop_dorian =     {0,3,4,5,7,9,10};
static const std::vector<Degree> blues =            {0,3,5,6,7,10};
static const std::vector<Degree> minor_pentatonic = {0,3,5,7,10};
static const std::vector<Degree> hungarian_minor =  {0,2,3,6,7,8,11};
static const std::vector<Degree> ukranian_dorian =  {0,2,3,6,7,9,10};
static const std::vector<Degree> marva =            {0,1,4,6,7,9,11};
static const std::vector<Degree> todi =             {0,1,3,6,7,8,11};
static const std::vector<Degree> whole_tone =       {0,2,4,6,8,10};
static const std::vector<Degree> choromatic =       {0,1,2,3,4,5,6,7,8,9,10,11};



static const std::vector<std::vector<Degree>> conversion_matrix = {
      natural_minor,
      major,
      dorian,
      phrygian,
      mixolydian,
      melodic_minor,
      harmonic_minor,
      bebop_dorian,
      blues,
      minor_pentatonic,
      hungarian_minor,
      ukranian_dorian,
      marva,
      todi,
      whole_tone,
      choromatic,
};

Degree GetScaleDegreeSize(const Scale &scale) {
  assert(scale < ScaleUnknown);
  const std::vector<Degree> &degree_table = conversion_matrix[scale];
  return degree_table.size();
}
Degree ScaleDegreeToChromatic(const Scale &scale, const Degree &degree) {
  assert(scale < ScaleUnknown);
  const std::vector<Degree> &degree_table = conversion_matrix[scale];
  assert(degree < degree_table.size());
  return degree_table[degree];
}

Note NoteToChromatic(const Note &note) {
  Note c = note;
  c.scale = ScaleChromatic;
  c.degree = ScaleDegreeToChromatic(note.scale, note.degree);
  return c;
}

Degree ConvertScale(const Scale &source_scale,
                    const Degree &degree,
                    const Scale &target_scale) {
  // we specialize the case of chromatic scale here because it will be frequently called.
  if (target_scale == ScaleChromatic) {
    return ScaleDegreeToChromatic(source_scale, degree);
  }
  assert(source_scale < ScaleUnknown && target_scale < ScaleUnknown);
  const std::vector<Degree> &source_degree_table = conversion_matrix[source_scale];
  const std::vector<Degree> &destination_degree_table = conversion_matrix[target_scale];
  assert(degree < source_degree_table.size());
  if (source_degree_table.size() == destination_degree_table.size()) {
    return degree;
  }
  // Snap the note.
  Degree choromatic_degree = ScaleDegreeToChromatic(source_scale, degree);
  int degree_difference = kOctaveDegree;
  Degree optimal_degree = 0;
  for (Degree i = 0; i < destination_degree_table.size(); ++i) {
    int diff = std::abs(destination_degree_table[i] - choromatic_degree);
    if (degree_difference > diff) {
      degree_difference = diff;
      optimal_degree = i;
    }
  }
  return optimal_degree;
}

Note ConvertNoteScale(const Note &note, const Scale &target_scale) {
  Note n = note;
  n.scale = target_scale;
  n.degree = ConvertScale(note.scale, note.degree, target_scale);
  return n;
}

MIDINote ScaleDegreeToMIDI(const Scale &scale,
                           const Degree &degree,
                           const Octave &octave,
                           const Degree &tonic_degree) {
  Degree chromatic_scale = ScaleDegreeToChromatic(scale, degree);
  MIDINote midi = (octave + kBaseOctave) * kOctaveDegree + chromatic_scale + tonic_degree;
  return midi;
}

MIDINote NoteToMIDI(const Note note, const Degree &tonic_degree) {
  return ScaleDegreeToMIDI(note.scale, note.degree, note.octave, tonic_degree);
}

Note MIDIToNote(const MIDINote &midi_note, const Scale &scale, const Degree &tonic_degree) {
  Note note;
  note.scale = scale;
  MIDINote midi = midi_note;
  midi -= tonic_degree;
  Degree chromatic_degree = midi % kOctaveDegree;
  note.octave = midi / kOctaveDegree - kBaseOctave;
  note.degree = ConvertScale(ScaleChromatic, chromatic_degree, scale);
  return note;
}

Octave MIDIOctave(const MIDINote &midi_note, const Degree &tonic_degree) {
  return MIDIToNote(midi_note, ScaleChromatic, tonic_degree).octave;
}

Degree MIDIToScaleDegree(const MIDINote &midi_note,
                         const Scale &scale,
                         const Degree &tonic_degree) {
  return MIDIToNote(midi_note, ScaleChromatic, tonic_degree).degree;
}


bool NoteEquals(const Note &base_note, const Note &target_note) {
  Note rebased_note = ConvertNoteScale(target_note, base_note.scale);
  return rebased_note.octave == base_note.octave && rebased_note.degree == base_note.degree;
}

bool NoteIdentical(const Note &note, const Note &another_note) {
  return note.octave == another_note.octave &&
         note.degree == another_note.degree &&
         note.scale == another_note.scale;
}
