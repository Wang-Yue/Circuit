//
//  Bridge.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

extern "C" {
#include "Bridge.h"
}

#include "CircuitController.hpp"
#include "MidiController.hpp"
#include "MIDIDelegate.hpp"
#include "ThreadSafeQueue.hpp"
#include <array>
#include <atomic>
#include <thread>
#include "NovationLaunchpadController.hpp"

class BridgeController : public MidiControllerDelegate {
public:
  BridgeController() {
    MidiController::GetInstance().SetDelegate(this);
    _circuit = new CircuitController();
    _circuit->Restart();
    _bpm = _circuit->GetBPM();
    _running = true;
    _thread = std::thread(&BridgeController::Loop, this);
    for (PadIndex i = 0; i < PadUnknown; ++i) {
      WritePadColor(i, 0);
      _pad_color[i] = 0;
    }
  }
  ~BridgeController() {
    _running = false;
    _thread.join() ;
    delete _circuit;
  }
  void Loop() {
    while (_running) {
      // beat per second
      const auto beat_duration = std::chrono::milliseconds(60 * 1000 / _bpm) ;
      // 1 beat is 1 quarter note, which is 24 midi ticks.
      const auto tick_duration = beat_duration / 24;
      std::this_thread::sleep_for(tick_duration);
      Tick();
    }
  }
  
  void Tick () {
    while (!_pad_command_queue.empty()) {
      PadCommand command = _pad_command_queue.front();
      _pad_command_queue.pop_front();
      if (command.tapped) {
        _circuit->GetPad(command.pad_index)->Tap();
      } else {
        _circuit->GetPad(command.pad_index)->Release();
      }
    }
    while (!_midi_note_queue.empty()) {
      MIDINotePacket command = _midi_note_queue.front();
      _midi_note_queue.pop_front();
      if (command.note_on) {
        _circuit->NoteOn(command.note, command.velocity);
      } else {
        _circuit->NoteOff(command.note);
      }
    }
    while (!_midi_control_queue.empty()) {
      MIDIControlPacket command = _midi_control_queue.front();
      _midi_control_queue.pop_front();
      _circuit->SendCC(command.control, command.cc);
    }
    _circuit->TickMicrostep();
    MidiController::GetInstance().Tick();
    for (PadIndex i = 0; i < PadUnknown; ++i) {
      ColorCode new_code = _circuit->GetPad(i)->GetColor().GetColorCode();
      if (new_code != _pad_color[i]) {
        WritePadColor(i, new_code);
        _pad_color[i] = new_code;
      }
    }
    _bpm = _circuit->GetBPM();
  }

  
  void TapPad(const PadIndex &pad_index) {
    PadCommand command = {
      .pad_index = pad_index,
      .tapped = true,
    };
    _pad_command_queue.push_back(command);
  }
  
  void ReleasePad(const PadIndex &pad_index) {
    PadCommand command = {
      .pad_index = pad_index,
      .tapped = false,
    };
    _pad_command_queue.push_back(command);
  }
  
  virtual void NoteOn(const unsigned char &midi_note, const unsigned char &velocity) override {
    printf("Note on: %d\n", midi_note);
    MIDINotePacket command = {
      .note = static_cast<MIDINote>(midi_note),
      .velocity = velocity,
      .note_on = true,
    };
    _midi_note_queue.push_back(command);
  }
  
  virtual void NoteOff(const unsigned char &midi_note) override {
    MIDINotePacket command = {
      .note = static_cast<MIDINote>(midi_note),
      .velocity = 0,
      .note_on = false,
    };
    _midi_note_queue.push_back(command);
  }
  virtual void Control(const unsigned char &control, const unsigned char &cc)  override {
    if (control >= kControlCapacity) {
      return;
    }
    MIDIControlPacket command = {
      .control = control,
      .cc = cc,
    };
    _midi_control_queue.push_back(command);
  }

  
private:
  CircuitController *_circuit;
  struct PadCommand {
    PadIndex pad_index;
    bool tapped;
  };
  ThreadSafeQueue<PadCommand> _pad_command_queue;
  
  struct MIDINotePacket {
    MIDINote note;
    Velocity velocity;
    bool note_on;
  };
  struct MIDIControlPacket {
    uint8_t control;
    CC cc;
  };
  ThreadSafeQueue<MIDINotePacket> _midi_note_queue;
  ThreadSafeQueue<MIDIControlPacket> _midi_control_queue;
  std::array<ColorCode, PadUnknown> _pad_color;
  BPM _bpm;
  std::thread _thread;
  std::atomic<bool> _running;
};


static BridgeController *controller = nullptr;



void InitializeCircuit(void) {
  controller = new BridgeController;
}

void TapPad(uint8_t pad) {
  controller->TapPad(pad);
}

void ReleasePad(uint8_t pad) {
  controller->ReleasePad(pad);
}

extern "C" {
void WriteNovationPadColor(const uint32_t pad, const uint32_t color);
void WriteCocoaPadColor(const uint32_t pad, const uint32_t color);

void WritePadColor(const uint32_t pad, const uint32_t color) {
  WriteNovationPadColor(pad, color);
  WriteCocoaPadColor(pad, color);
}
}
