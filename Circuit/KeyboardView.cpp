//
//  KeyboardView.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "KeyboardView.hpp"

#include "Pad.hpp"
#include "Scale.hpp"
#include <map>

static const Color kAvailableKeyColor(0x0000ffff);
static const Color kOctaveKeyColor(0x00ff00ff);
static const Color kEnabledKeyColor(0x00ffff00);
static const Color kDisabledKeyColor(0x00000000);

class KeyboardView::OneOctaveKeyboardView : public PadDelegate {
public:
  OneOctaveKeyboardView(const Note &base_note,
               const Degree &tonic_degree,
               const std::vector<Pad *> &pads,
               KeyboardViewDelegate *delegate) :
  _base_note(base_note),
  _tonic_degree(tonic_degree),
  _delegate(delegate),
  _pads(shufflePads(pads, base_note.scale)) {
    assert(base_note.degree == 0);
    assert(base_note.scale < ScaleUnknown);
    Degree size = GetScaleDegreeSize(base_note.scale);
    for (Degree i = 0; i <= size; ++i) {
      Pad *pad = _pads[i];
      Note note = base_note;
      if (i == size) {
        note.degree = 0;
        note.octave = base_note.octave + 1;
      } else {
        note.degree = i;
      }
      // test if note is within midi range.
      MIDINote midi = NoteToMIDI(note, _tonic_degree);
      if (midi >= 0) {
        PadIndex pad_index = pad->GetPadIndex();
        _pad_note_mapping[pad_index] = note;
        pad->SetDelegate(this);
      } else {
        pad->SetColor(kDisabledKeyColor);
      }
    }
    for (PadIndex index = size + 1; index < _pads.size(); ++index) {
      _pads[index]->SetColor(kDisabledKeyColor);
      _pads[index]->SetDelegate(this);
    }
  }
  ~OneOctaveKeyboardView() {
    for (Pad *pad : _pads) {
      pad->SetDelegate(nullptr);
    }
  }
  const std::vector<Pad *> shufflePads(const std::vector<Pad *> &pads, const Scale &scale) {
    if (scale == ScaleChromatic) {
      assert(pads.size() == kPadsInRow * 2);
      static const PadIndex matrix[] = {8,1,9,2,10,11,4,12,5,13,6,14,15, 0,3,7};
      std::vector<Pad *> result;
      for (PadIndex pad = 0; pad < pads.size() + 1; ++pad) {
        result.push_back(pads[matrix[pad]]);
      }
      return result;
    } else {
      // No need to shuffle.
      assert(pads.size() == kPadsInRow);
      return pads;
    }
  }
  PadIndex ActivePadCount() const {
    return _pad_note_mapping.size();
  }
  Pad *GetPad(const Note &note) {
    Note n = ConvertNoteScale(note, _base_note.scale);
    if (n.octave == _base_note.octave + 1 && n.degree == 0) {
      Degree size = GetScaleDegreeSize(_base_note.scale);
      return _pads[size];
    }
    if (n.octave != _base_note.octave) {
      return nullptr;
    }
    return _pads[n.degree];
  }
  
  void SetPadColor(const Note &note, const Color &color) {
    Pad *pad = GetPad(note);
    if (!pad) {
      return;
    }
    pad->SetColor(color);
  }

  void AllNotesOff() {
    Degree scale_degree_size = GetScaleDegreeSize(_base_note.scale);
    for (Degree degree = 0; degree <=  scale_degree_size; ++degree) {
      // Pads not in _pad_note_mapping are out of midi range.
      PadIndex pad_index = _pads[degree]->GetPadIndex();

      if (_pad_note_mapping.find(pad_index) == _pad_note_mapping.end()) {
        _pads[degree]->SetColor(kDisabledKeyColor);
      } else if (degree == 0 || degree == scale_degree_size) {
        _pads[degree]->SetColor(kOctaveKeyColor);
      } else {
        _pads[degree]->SetColor(kAvailableKeyColor);
      }
    }
  }
  
  virtual void Tap(Pad *pad) override {
    PadIndex pad_index = pad->GetPadIndex();
    if (_pad_note_mapping.find(pad_index) == _pad_note_mapping.end()) {
      return;
    }
    Note note = _pad_note_mapping[pad_index];
    if (_delegate) {
      _delegate->Tap(note);
    }
  }
  virtual void Release(Pad *pad) override {
    PadIndex pad_index = pad->GetPadIndex();
    if (_pad_note_mapping.find(pad_index) == _pad_note_mapping.end()) {
      return;
    }
    Note note = _pad_note_mapping[pad_index];
    if (_delegate) {
      _delegate->Release(note);
    }
  }
private:
  const Note _base_note;
  const Degree _tonic_degree;
  const std::vector<Pad *> _pads;
  // This is the real pad index stored. Never ever use it for _pads ordering.
  std::map<PadIndex, Note> _pad_note_mapping;
  KeyboardViewDelegate *_delegate;
};

KeyboardView::KeyboardView(const Note &base_note,
                           const Degree &tonic_degree,
                           const std::vector<Pad *> &pads,
                           KeyboardViewDelegate *delegate) :  _delegate(delegate) {
  assert(base_note.degree == 0);
  assert(base_note.scale < ScaleUnknown);
  PadIndex octave_pad_count = kPadsInRow;
  if (base_note.scale == ScaleChromatic) {
    octave_pad_count *= 2;
  }
  assert(pads.size() % octave_pad_count == 0);
  Note current_base_note = base_note;
  for (PadIndex start_pad = 0; start_pad < pads.size(); start_pad += octave_pad_count) {
    std::vector<Pad *> octave_pads;
    for (PadIndex i = 0; i < octave_pad_count; ++i) {
      octave_pads.push_back(pads[start_pad + i]);
    }
    OneOctaveKeyboardView *view =
        new OneOctaveKeyboardView(current_base_note, tonic_degree, octave_pads, this);
    _views.push_back(view);
    --current_base_note.octave;
  }
}

KeyboardView::~KeyboardView() {
  for (OneOctaveKeyboardView *view : _views) {
    delete view;
  }
}

std::vector<Pad *> KeyboardView::GetPads(const Note &note) {
  std::vector<Pad *> pads;
  for (OneOctaveKeyboardView *view : _views) {
    Pad *pad = view->GetPad(note);
    if (pad) {
      pads.push_back(pad);
    }
  }
  return pads;
}

void KeyboardView::AllNotesOff() {
  for (OneOctaveKeyboardView *view : _views) {
    view->AllNotesOff();
  }
}

void KeyboardView::Highlight(const Note &note) {
  for (OneOctaveKeyboardView *view : _views) {
    view->SetPadColor(note, kEnabledKeyColor);
  }
}
  
void KeyboardView::Tap(const Note &note)  {
  if (_delegate) {
    _delegate->Tap(note);
  }
}

void KeyboardView::Release(const Note &note) {
  if (_delegate) {
    _delegate->Release(note);
  }
}


