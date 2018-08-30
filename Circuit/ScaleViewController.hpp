//
//  ScaleViewController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/26/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef ScaleViewController_hpp
#define ScaleViewController_hpp


#include "ScreenController.hpp"

#include "TypeDefs.hpp"

#include "KeyboardView.hpp"
#include "ScaleSelectionView.hpp"

#include <vector>
#include <set>

class CircuitController;


class ScaleViewController :
public ScreenController,
public ScaleSelectionViewDelegate,
public KeyboardViewDelegate {
public:
  ScaleViewController(CircuitController *parent);
  virtual ~ScaleViewController();
  virtual void Update() override;
  virtual void UpdateRunningMode() override;
  virtual void UpdateEditingMode() override;
  virtual void Tap(const Scale &scale) override;
  virtual void Tap(const Note &note) override;
  virtual void Release(const Note &note) override;
private:
  KeyboardView *_keyboard_view;
  ScaleSelectionView *_scale_selection_view;
};

#endif /* ScaleViewController_hpp */
