//
//  ScaleSelectionView.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/26/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef ScaleSelectionView_hpp
#define ScaleSelectionView_hpp

#include "TypeDefs.hpp"
#include "Pad.hpp"
#include <vector>
#include <map>

class ScaleSelectionViewDelegate {
public:
  virtual void Tap(const Scale &scale) = 0;
};

class ScaleSelectionView : public PadDelegate {
public:
  ScaleSelectionView(const std::vector<Pad *> &pads, ScaleSelectionViewDelegate *delegate);
  ~ScaleSelectionView();
  void SetScale(const Scale &scale);
  virtual void Tap(Pad *pad) override;
  virtual void Release(Pad *pad) override;
private:
  ScaleSelectionViewDelegate *_delegate;
  std::vector<Pad *> _pads;
  std::map<PadIndex, Scale> _scale_mapping;
};
#endif /* ScaleSelectionView_hpp */
