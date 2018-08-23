//
//  Scale.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef Scale_hpp
#define Scale_hpp

#include "TypeDefs.hpp"

Degree GetScaleDegreeSize(const enum Scale &scale);

// Assuming same base note.
Degree ScaleDegreeToChromatic(const enum Scale &scale, const Degree &degree);
Note NoteToChromatic(const Note &note);

// Assuming same base note.
Degree ConvertScale(const enum Scale &source_scale,
                    const Degree &degree,
                    const enum Scale &target_scale);
Note ConvertNoteScale(const Note &note, const enum Scale &target_scale);

MIDINote ScaleDegreeToMIDI(const enum Scale &scale,
                         const Degree &degree,
                         const Octave &octave,
                         const Degree &tonic_degree);
MIDINote NoteToMIDI(const Note note, const Degree &tonic_degree);

Note MIDIToNote(const MIDINote &midi_note, const enum Scale &scale, const Degree &tonic_degree);

Octave MIDIOctave(const MIDINote &midi_note, const Degree &tonic_degree);

Degree MIDIToScaleDegree(const MIDINote &midi_note,
                         const enum Scale &scale,
                         const Degree &tonic_degree);

// Test target note on the base note's scale, and see if they are the same note.
bool NoteEquals(const Note &base_note, const Note &target_note);

// Test and see if all fields are the same.
bool NoteIdentical(const Note &base_note, const Note &target_note);


#endif /* Scale_hpp */
