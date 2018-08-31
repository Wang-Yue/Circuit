//
//  NovationLaunchpadController.cpp
//  CircuitApp
//
//  Created by Yue Wang on 8/29/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#include "NovationLaunchpadController.hpp"

extern "C" {
#include "Bridge.h"
  
void WriteNovationPadColor(const uint32_t pad, const uint32_t color) {
  NovationLaunchpadController::GetInstance().SendColorMessage(pad, color);
}

}

static void callback( double deltatime, std::vector< unsigned char > *message, void *userData ) {
  size_t nBytes = message->size();
  if (nBytes && message->at(0) == 144) {
    char key = message->at(1);
    char row = key % 16;
    char column = key / 16;
    char pad = row + column * 8;
    printf("Tap pad {%d, %d}\n", row, column);
    if (message->at(2)) {
      TapPad(pad);
    } else {
      ReleasePad(pad);
    }
  }
}


NovationLaunchpadController &NovationLaunchpadController::GetInstance() {
  static NovationLaunchpadController instance;
  return instance;
}

void NovationLaunchpadController::SendColorMessage(const char &pad, const  uint32_t &colorCode) {
  uint8_t column = pad % 8;
  uint8_t row = pad / 8;
  uint8_t key = (16 * row) + column;
  uint8_t r = colorCode & 0xff;
  uint8_t g = (colorCode >> 8) & 0xff;
  // uint8_t b = (colorCode >> 16) & 0xff;
  uint8_t velocity = (16 * (g / 64)) + (r / 64) + 12;
  char v = static_cast<char>(velocity);
  std::vector<unsigned char> message;
  message.push_back(144);
  message.push_back(key);
  message.push_back(v);
  _midi_out->sendMessage( &message );
}

NovationLaunchpadController::NovationLaunchpadController() : _midi_in(0), _midi_out(0) {
  // RtMidiIn constructor
  try {
    _midi_in = new RtMidiIn();
  }
  catch ( RtMidiError &error ) {
    error.printMessage();
    exit( EXIT_FAILURE );
  }
  // Check inputs.
  unsigned int nPorts = _midi_in->getPortCount();
  std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
  std::string portName;
  for ( unsigned int i=0; i<nPorts; i++ ) {
    try {
      portName = _midi_in->getPortName(i);
      if (portName.find("Launchpad Mini")!=std::string::npos) {
        _midi_in->openPort(i);
      }
      _midi_in->setCallback(&callback);
      _midi_in->ignoreTypes(true, true, true);
    }
    catch ( RtMidiError &error ) {
      error.printMessage();
      delete _midi_in;
    }
    std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
  }
  // RtMidiOut constructor
  try {
    _midi_out = new RtMidiOut();
  }
  catch ( RtMidiError &error ) {
    error.printMessage();
    exit( EXIT_FAILURE );
  }
  // Check outputs.
  nPorts = _midi_out->getPortCount();
  std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";
  for ( unsigned int i=0; i<nPorts; i++ ) {
    try {
      portName = _midi_out->getPortName(i);
      if (portName.find("Launchpad Mini")!=std::string::npos) {
        _midi_out->openPort(i);
      }
    }
    catch (RtMidiError &error) {
      error.printMessage();
      delete _midi_out;
    }
    std::cout << "  Output Port #" << i+1 << ": " << portName << '\n';
  }
  
  
}
