//
//  Bridge.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "Bridge.hpp"
#include "CircuitController.hpp"
#include "MidiController.hpp"

static CircuitController *controller = nullptr;

void InitializeCircuit() {
  MidiController::getInstance();
  controller = new CircuitController();
  controller->RestartRunning();
}
void PressKey(int key) {
  controller->GetPad(key)->Tap();
}
void ReleaseKey(int key) {
  controller->GetPad(key)->Release();
}

uint32_t GetColor(int key) {
  return controller->GetPad(key)->GetColor().GetColorCode();
}

void Tick() {
  controller->TickStep();
  MidiController::getInstance().Tick();
}
