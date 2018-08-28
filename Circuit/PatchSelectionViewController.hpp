//
//  PatchSelectionViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/27/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef PatchSelectionViewController_hpp
#define PatchSelectionViewController_hpp

#include <map>
#include <vector>
#include "Channel.hpp"
#include "Pad.hpp"

class AtomClass;

static const Color kHighlightColor(0x0000007f);
static const Color kEnabledColor(0x0000003f);

template <typename AtomClass = Atom, typename AtomPatchIndex = uint8_t>
class PatchSelectionViewControllerDelegate {
public:
  virtual void TapPatch(const AtomPatchIndex &index) = 0;
  virtual void ReleasePatch(const AtomPatchIndex &index) = 0;
};

template <typename AtomClass = Atom, typename AtomPatchIndex = uint8_t>
class PatchSelectionViewController : public PadDelegate {
public:
  PatchSelectionViewController(const std::vector<Pad *> &pads,
      PatchSelectionViewControllerDelegate<AtomClass, AtomPatchIndex> *delegate);
  virtual ~PatchSelectionViewController();
  void HandleOctUp();
  void HandleOctDown();
  
  void SetSelectedAtomPatchIndex(const bool &highlight, const AtomPatchIndex &index);
  void Update();
  // PadDelegate.
  virtual void Tap(Pad *pad) override;
  virtual void Release(Pad *pad) override;
  
  
private:
  bool _first_page;
  bool _highlight_selected;
  AtomPatchIndex _selected_index;
  PatchSelectionViewControllerDelegate<AtomClass, AtomPatchIndex> *_delegate;
  std::vector<Pad *> _pads;
};

template <typename AtomClass, typename AtomPatchIndex>
PatchSelectionViewController<AtomClass, AtomPatchIndex>::PatchSelectionViewController(
    const std::vector<Pad *> &pads,
    PatchSelectionViewControllerDelegate<AtomClass, AtomPatchIndex> *delegate) :
_pads(pads), _delegate(delegate), _first_page(true), _highlight_selected(false) {
  assert(pads.size() == kRegularPadCount);
  for (Pad *pad: _pads) {
    pad->SetDelegate(this);
  }
}

template <typename AtomClass, typename AtomPatchIndex>
void PatchSelectionViewController<AtomClass, AtomPatchIndex>::HandleOctUp() {
  if (!_first_page) {
    _first_page = true;
  }
}

template <typename AtomClass, typename AtomPatchIndex>
void PatchSelectionViewController<AtomClass, AtomPatchIndex>::HandleOctDown() {
  if (_first_page) {
    _first_page = false;
  }
}

template <typename AtomClass, typename AtomPatchIndex>
PatchSelectionViewController<AtomClass, AtomPatchIndex>::~PatchSelectionViewController() {
  for (Pad *pad: _pads) {
    pad->SetDelegate(nullptr);
  }
}

template <typename AtomClass, typename AtomPatchIndex>
void PatchSelectionViewController<AtomClass, AtomPatchIndex>::Tap(Pad *pad) {
  if (_delegate) {
    PadIndex pad_index = pad->GetPadIndex();
    AtomPatchIndex AtomClass_index = pad_index;
    if (!_first_page) {
      AtomClass_index += kRegularPadCount;
    }
    _delegate->TapPatch(AtomClass_index);
  }
}

template <typename AtomClass, typename AtomPatchIndex>
void PatchSelectionViewController<AtomClass, AtomPatchIndex>::Release(Pad *pad) {
  if (_delegate) {
    PadIndex pad_index = pad->GetPadIndex();
    AtomPatchIndex AtomClass_index = pad_index;
    if (!_first_page) {
      AtomClass_index += kRegularPadCount;
    }
    _delegate->ReleasePatch(AtomClass_index);
  }
}

template <typename AtomClass, typename AtomPatchIndex>
void PatchSelectionViewController<AtomClass, AtomPatchIndex>::SetSelectedAtomPatchIndex(
    const bool &highlight,
    const AtomPatchIndex &index) {
  _highlight_selected = highlight;
  _selected_index = index;
}

template <typename AtomClass, typename AtomPatchIndex>
void PatchSelectionViewController<AtomClass, AtomPatchIndex>::Update() {
  for (Pad *pad : _pads) {
    PadIndex pad_index = pad->GetPadIndex();
    AtomPatchIndex AtomClass_index = pad_index;
    if (!_first_page) {
      AtomClass_index += kRegularPadCount;
    }
    if (_highlight_selected && AtomClass_index == _selected_index){
      pad->SetColor(kHighlightColor);
    } else {
      pad->SetColor(kEnabledColor);
    }
  }
}

#include "TypeDefs.hpp"
class Sample;
class Synth;

using SamplePatchSelectionViewControllerDelegate = PatchSelectionViewControllerDelegate<Sample, SampleIndex>;
using SamplePatchSelectionViewController = PatchSelectionViewController<Sample, SampleIndex>;
using SynthPatchSelectionViewControllerDelegate = PatchSelectionViewControllerDelegate<Synth, SynthIndex>;
using SynthPatchSelectionViewController = PatchSelectionViewController<Synth, SynthIndex>;

#endif /* PatchSelectionViewController_hpp */
