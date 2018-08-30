//
//  NovationLaunchpadController.hpp
//  CircuitApp
//
//  Created by Yue Wang on 8/29/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef NovationLaunchpadController_hpp
#define NovationLaunchpadController_hpp

#include <cstdint>
#include <RtMidi.h>

extern "C" {

#include "Bridge.h"

}
class NovationLaunchpadController {
public:
  NovationLaunchpadController(NovationLaunchpadController const&) = delete;
  void operator=(NovationLaunchpadController const&)  = delete;

  static NovationLaunchpadController& GetInstance() {
    static NovationLaunchpadController instance;
    return instance;
  }
  void SendColorMessage(const char &pad, const  uint32_t &colorCode){
    uint32_t column = pad % 8;
    uint32_t row = pad / 8;
    uint32_t key = (16 * row) + column;
    uint32_t r = colorCode & 0xff;
    uint32_t g = (colorCode >> 8) & 0xff;
   // char b = (colorCode >> 16) & 0xff;
    uint32_t velocity = (16 * (g / 64)) + (r / 64) + 12;
    char v = static_cast<char>(velocity);
    std::vector<unsigned char> message;
    message.push_back(144);
    message.push_back(key);
    message.push_back(v);
    printf("Display color {%d, %d}\n", row, column);
    _midi_out->sendMessage( &message );
  };
  
  void Tick() {
    while (ReceiveNoteMessage()) {
    }
  }
  
  const bool ReceiveNoteMessage(){
    std::vector<unsigned char> message;
    _midi_in->ignoreTypes( true, true, true );
    _midi_in->getMessage( &message );
    size_t nBytes = message.size();
    if (nBytes && message[0] == 144) {
        char key = message[1];
        char row = key % 16;
        char column = key / 16;
        char pad = row + column * 8;
      printf("Tap pad {%d, %d}\n", row, column);
      if (message[2]) {
        TapPad(pad);
      } else {
        ReleasePad(pad);
      }
        return true;
    }
//    if (nBytes && message[0] == 176) {
//        _delegate->Control(message[1], message[2]);
//        return true;
//    }
    return false;
  }
private:
  NovationLaunchpadController() {
    
    _midi_in = 0;
    _midi_out = 0;
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
  RtMidiIn  *_midi_in = 0;
  RtMidiOut *_midi_out = 0;
};

#endif /* NovationLaunchpadController_hpp */
